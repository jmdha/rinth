#include <assert.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#include "domain.h"
#include "lexer.h"
#include "log.h"

static void parse_name(string *name) {
    assert(lexer_next(name) == KIND_ID);
    lexer_expect(KIND_RPAREN);
}

static void parse_requirements(string *requirements, uint *count) {
    while (lexer_next(&requirements[*count]) == KIND_ID)
        (*count)++;
}

static void parse_predicates(Predicate *predicates, uint *count) {
    string tmp;
    while (lexer_next(&tmp) == KIND_LPAREN) {
        Predicate *predicate = &predicates[(*count)++];
        assert(lexer_next(&predicate->name) == KIND_ID);
        predicate->var_count = 0;
        while (lexer_next(&predicate->vars[predicate->var_count]) == KIND_ID)
            predicate->var_count++;
    }
}

static void parse_parameters(string *vars, uint *count) {
    *count = 0;
    while (lexer_next(&vars[*count]) == KIND_ID)
        (*count)++;
}

static void parse_expression(Expression **exp) {
    string tmp = {0};
    enum kind kind = lexer_next(&tmp);
    if (kind == KIND_RPAREN) return;
    *exp = malloc(sizeof(Expression));
    switch (keyword_match(&tmp)) {
    case KEYWORD_NOT:
        (*exp)->kind = E_NOT;
        lexer_expect(KIND_LPAREN);
        parse_expression(&(*exp)->data.unary);
        lexer_expect(KIND_RPAREN);
        break;
    case KEYWORD_AND:
        (*exp)->kind = E_AND;
        (*exp)->data.nary.count = 0;
        while (lexer_next(&tmp) == KIND_LPAREN)
            parse_expression(&(*exp)->data.nary.exps[(*exp)->data.nary.count++]);
        break;
    case KEYWORD_OR:
        (*exp)->kind = E_OR;
        (*exp)->data.nary.count = 0;
        while (lexer_next(&tmp) == KIND_LPAREN)
            parse_expression(&(*exp)->data.nary.exps[(*exp)->data.nary.count++]);
        break;
    default:
        (*exp)->kind = E_ATOM;
        (*exp)->data.atom.predicate = tmp;
        (*exp)->data.atom.var_count = 0;
        while (lexer_next(&(*exp)->data.atom.vars[(*exp)->data.atom.var_count]) == KIND_ID)
            (*exp)->data.atom.var_count++;
        break;
    }
}

static void parse_action(Action *action) {
    assert(lexer_next(&action->name) == KIND_ID);
    lexer_expect_def(KEYWORD_PARAMETERS);
    lexer_expect(KIND_LPAREN);
    parse_parameters(action->vars, &action->var_count);
    lexer_expect_def(KEYWORD_PRECONDITION);
    lexer_expect(KIND_LPAREN);
    parse_expression(&action->precondition);
    lexer_expect_def(KEYWORD_EFFECT);
    lexer_expect(KIND_LPAREN);
    parse_expression(&action->effect);
    lexer_expect(KIND_RPAREN);
}

void domain_parse_(Domain *domain, const char *str) {
    domain->name.ptr          = NULL;
    domain->requirement_count = 0;
    domain->predicate_count   = 0;
    domain->action_count      = 0;
    lexer_init(str);
    lexer_expect(KIND_LPAREN);
    lexer_expect_def(KEYWORD_DEFINE);
    string tmp = {0};
    enum kind kind;
    while ((kind = lexer_next(&tmp)) != KIND_EOI) {
        if (kind == KIND_RPAREN) break;
        assert(kind == KIND_LPAREN);
        lexer_next(&tmp);
        enum keyword keyword = keyword_match(&tmp);
        TRACE("Parse %s", KEYWORD_NAMES[keyword]);
        switch (keyword) {
        case KEYWORD_NAME: parse_name(&domain->name); break;
        case KEYWORD_REQUIREMENTS:
            parse_requirements(domain->requirements, &domain->requirement_count);
            break;
        case KEYWORD_PREDICATES:
            parse_predicates(domain->predicates, &domain->predicate_count);
            break;
        case KEYWORD_ACTION: parse_action(&domain->actions[domain->action_count++]); break;
        default: 
            fprintf(stderr, "%s: Unexpected keyword\n", KEYWORD_NAMES[keyword]);
            exit(1);
        }
    }
    INFO("Domain:     %.*s", domain->name.len, domain->name.ptr);
    INFO("Predicates: %d", domain->predicate_count);
    INFO("Actions:    %d", domain->action_count);
}

Domain domain_parse(const char *str) {
    Domain domain;
    domain_parse_(&domain, str);
    return domain;
}
