#include <memory.h>
#include <stdlib.h>

#include "domain.h"
#include "lexer.h"
#include "log.h"

static void ParseList(string *list, uint *count, TokenKind kind, Token *t) {
    *count = 0;
    while (LexerNext(t)) {
        if (t->kind == kind)
            list[(*count)++] = t->str;
        else if (t->kind == RPAREN)
            return;
        else
            ExpectEither(t->kind, kind, RPAREN);
    }
    EOI(RPAREN);
}

static void ParseName(string *name, Token *t) {
    ExpectNext(t, ID);
    *name = t->str;
    ExpectNext(t, RPAREN);
}

static void ParseRequirements(string *list, uint *count, Token *t) {
    while (LexerNext(t)) {
        if (t->kind == RPAREN) return;
        bool is_requirement;
        switch (t->kind) {
        case REQ_STRIPS: is_requirement = true; break;
        default: is_requirement = false; break;
        }
        if (!is_requirement) Expected("REQUIREMENT or RPAREN", t->kind);
        list[(*count)++] = t->str;
    }
    EOI(RPAREN);
}

static void ParsePredicate(Predicate *predicate, Token *t) {
    ExpectNext(t, ID);
    predicate->name = t->str;
    ParseList(predicate->vars, &predicate->var_count, VARIABLE, t);
}

static void ParsePredicates(Predicate *list, uint *count, Token *t) {
    while (LexerNext(t)) {
        if (t->kind == LPAREN)
            ParsePredicate(&(list[(*count)++]), t);
        else if (t->kind == RPAREN)
            return;
        else
            ExpectEither(t->kind, LPAREN, RPAREN);
    }
    EOI(RPAREN);
}

static void ParseExpression(Expression **exp, Token *t) {
    if (!LexerNext(t)) EOI(RPAREN);
    if (t->kind == RPAREN) return;
    Expression *e = malloc(sizeof(Expression));
    *exp          = e;
    switch (t->kind) {
    case ID:
        e->kind = E_ATOM;
        e->data.atom.predicate = t->str;
        ParseList(e->data.atom.vars, &e->data.atom.var_count, VARIABLE, t);
        break;
    case EXP_NOT:
        e->kind = E_NOT;
        ExpectNext(t, LPAREN);
        ParseExpression(&e->data.unary, t);
        ExpectNext(t, RPAREN);
        break;
    case EXP_OR: e->kind = E_OR; goto C_NARY;
    case EXP_AND:
        e->kind = E_AND;
    C_NARY:
        e->data.nary.count = 0;
        while (LexerNext(t)) {
            if (t->kind == RPAREN) return;
            if (t->kind != LPAREN) Expected("LPAREN", t->kind);
            ParseExpression(&e->data.nary.exps[e->data.nary.count++], t);
        }
        ExpectNext(t, RPAREN);
        break;
    default: free(e); Expected("EXPRESSION or RPAREN", t->kind);
    };
}

static void ParseAction(Action *action, Token *t) {
    ExpectNext(t, ID);
    action->name = t->str;
    TRACE("Parsing action %s", action->name);
    ExpectNext(t, DEF_PARAMETERS);
    TRACE("Parsing parameters of %s", action->name);
    ExpectNext(t, LPAREN);
    ParseList(action->vars, &action->var_count, VARIABLE, t);
    ExpectNext(t, DEF_PRECONDITION);
    TRACE("Parsing precondition of %s", action->name);
    ExpectNext(t, LPAREN);
    ParseExpression(&action->precondition, t);
    ExpectNext(t, DEF_EFFECT);
    TRACE("Parsing effect of %s", action->name);
    ExpectNext(t, LPAREN);
    ParseExpression(&action->effect, t);
    ExpectNext(t, RPAREN);
}

Domain DomainParse(const char *str) {
    LexerInit(str);
    Domain domain = {.name = NULL, .predicate_count = 0, .action_count = 0};
    Token t;

    ExpectNext(&t, LPAREN);
    ExpectNext(&t, DEF_DEFINE);
    while (LexerNext(&t)) {
        if (t.kind == RPAREN) break;
        Expect(t.kind, LPAREN);
        LexerNext(&t);
        TRACE("Parsing %s token in domain parsing", TOKEN_NAMES[t.kind]);
        switch (t.kind) {
        case DEF_NAME: ParseName(&domain.name, &t); break;
        case DEF_REQUIREMENTS:
            ParseRequirements(domain.requirements, &domain.requirement_count, &t);
            break;
        case DEF_PREDICATES:
            ParsePredicates(domain.predicates, &domain.predicate_count, &t);
            break;
        case DEF_ACTION: ParseAction(&domain.actions[domain.action_count++], &t); break;
        default: ERROR("Unexpected token %s", TOKEN_NAMES[t.kind]); exit(1);
        }
    }

    INFO("Predicates: %d", domain.predicate_count);
    INFO("Actions: %d", domain.action_count);

    if (domain.predicate_count > MAX_PREDICATES * 0.5)
        WARN("Domain has %d predicates, which is near maximum.", domain.predicate_count);
    if (domain.action_count > MAX_ACTIONS * 0.5)
        WARN("Domain has %d actions, which is near maximum.", domain.action_count);

    return domain;
}
