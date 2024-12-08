#include <memory.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "lexer.h"
#include "log.h"
#include "problem.h"

static void parse_name(string *name) {
    lexer_next(name);
    lexer_expect(KIND_RPAREN);
}

static void parse_objects(string *objects, uint *count) {
    while (lexer_next(&objects[*count]) != KIND_RPAREN)
        (*count)++;
}

static void parse_facts(SFact *facts, uint *count) {
    enum kind kind;
    string tmp;
    while ((kind = lexer_next(&tmp)) == KIND_LPAREN) {
        SFact *fact = &facts[(*count)++];
        if ((kind = lexer_next(&fact->predicate)) != KIND_ID) {
            ERROR("Expected predicate name in fact found %s", KIND_NAMES[kind]);
            abort();
        }
        while ((kind = lexer_next(&fact->args[fact->arg_count])) != KIND_RPAREN)
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
    memset(problem, 0, sizeof(Problem));
    lexer_init(str);
    lexer_expect(KIND_LPAREN);
    lexer_expect_def(KEYWORD_DEFINE);
    string tmp;
    enum kind kind;
    while ((kind = lexer_next(&tmp)) != KIND_EOI) {
        if (kind == KIND_RPAREN) break;
        if (kind != KIND_LPAREN) {
            ERROR("Expected top level L paren, found %s", KIND_NAMES[kind]);
            abort();
        }
        lexer_next(&tmp);
        enum keyword keyword;
        switch ((keyword = keyword_match(&tmp))) {
        case KEYWORD_NAME: parse_name(&problem->name); break;
        case KEYWORD_DOMAIN: parse_name(&problem->domain); break;
        case KEYWORD_OBJECTS: parse_objects(problem->objects, &problem->object_count); break;
        case KEYWORD_INIT: parse_init(problem->inits, &problem->init_count); break;
        case KEYWORD_GOAL: parse_goal(problem->goals, &problem->goal_count); break;
        default: ERROR("Unexpected token %s", KEYWORD_NAMES[keyword]); abort();
        }
    }
}

void ProblemPrint(Problem *problem) {
    printf("Domain: %.*s\n", problem->domain.len, problem->domain.ptr);
    printf("Name: %.*s\n", problem->name.len, problem->name.ptr);
    printf("Object count: %d\n", problem->object_count);
    printf("Objects:\n");
    for (uint i = 0; i < problem->object_count; i++)
        printf("\t%.*s\n", problem->objects[i].len, problem->objects[i].ptr);
    printf("Init count: %d\n", problem->init_count);
    printf("Inits:\n");
    for (uint i = 0; i < problem->init_count; i++) {
        printf("\t%.*s", problem->inits[i].predicate.len, problem->inits[i].predicate.ptr);
        for (uint t = 0; t < problem->inits[i].arg_count; t++)
            printf(" %.*s", problem->inits[i].args[t].len, problem->inits[i].args[t].ptr);
        printf("\n");
    }
    printf("Goal count: %d\n", problem->goal_count);
    printf("Goals:\n");
    for (uint i = 0; i < problem->goal_count; i++) {
        printf("\t%.*s", problem->goals[i].predicate.len, problem->goals[i].predicate.ptr);
        for (uint t = 0; t < problem->goals[i].arg_count; t++)
            printf(" %.*s", problem->goals[i].args[t].len, problem->goals[i].args[t].ptr);
        printf("\n");
    }
}
