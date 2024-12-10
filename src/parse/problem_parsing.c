#include <memory.h>
#include <stdbool.h>
#include <stdlib.h>
#include <strings.h>
#include <assert.h>

#include "lexer.h"
#include "log.h"
#include "problem.h"

static void parse_name(string *name) {
    assert(lexer_next(name) == KIND_ID);
    lexer_expect(KIND_RPAREN);
}

static void parse_objects(string *objects, uint *count) {
    while (lexer_next(&objects[*count]) == KIND_ID)
        (*count)++;
}

static void parse_facts(SFact *facts, uint *count) {
    string tmp;
    while (lexer_next(&tmp) == KIND_LPAREN) {
        SFact *fact = &facts[(*count)++];
        assert(lexer_next(&fact->predicate) == KIND_ID);
        fact->arg_count = 0;
        while (lexer_next(&fact->args[fact->arg_count]) == KIND_ID)
            fact->arg_count++;
    }
}

static void parse_init(SFact *facts, uint *count) { parse_facts(facts, count); }

static void parse_goal(SFact *facts, uint *count) {
    lexer_expect(KIND_LPAREN);
    lexer_expect_def(KEYWORD_AND);
    parse_facts(facts, count);
    lexer_expect(KIND_RPAREN);
}

void ProblemParse(Problem *problem, const char *str) {
    problem->name.len     = 0;
    problem->domain.len   = 0;
    problem->object_count = 0;
    problem->goal_count   = 0;
    problem->init_count   = 0;
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
        TRACE("Parsing %s", KEYWORD_NAMES[keyword]);
        switch (keyword) {
        case KEYWORD_NAME: parse_name(&problem->name); break;
        case KEYWORD_DOMAIN: parse_name(&problem->domain); break;
        case KEYWORD_OBJECTS: parse_objects(problem->objects, &problem->object_count); break;
        case KEYWORD_INIT: parse_init(problem->inits, &problem->init_count); break;
        case KEYWORD_GOAL: parse_goal(problem->goals, &problem->goal_count); break;
        default: ERROR("Unexpected top level keyword %s", KEYWORD_NAMES[keyword]); abort();
        }
    }
}
