#include <memory.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "lexer.h"
#include "log.h"
#include "problem.h"

static void ParseName(string *name, Token *t) {
    ExpectNext(t, ID);
    *name = t->str;
    ExpectNext(t, RPAREN);
}

static void ParseList(string *list, uint *count, Token *t) {
    *count = 0;
    while (LexerNext(t)) {
        if (t->kind == ID)
            list[(*count)++] = t->str;
        else if (t->kind == RPAREN)
            return;
        else
            ExpectEither(t->kind, ID, RPAREN);
    }
    EOI(RPAREN);
}

static void ParseFact(SFact *fact, Token *t) {
    ExpectNext(t, ID);
    fact->predicate = t->str;
    ParseList(fact->args, &fact->arg_count, t);
}

static void ParseFacts(SFact *fact_list, uint *count, Token *t) {
    while (LexerNext(t)) {
        if (t->kind == LPAREN)
            ParseFact(&(fact_list[(*count)++]), t);
        else if (t->kind == RPAREN)
            return;
        else
            ExpectEither(t->kind, LPAREN, RPAREN);
    }
    EOI(RPAREN);
}

// TODO: Handle other things than AND
static void ParseGoal(SFact *fact_list, uint *count, Token *t) {
    ExpectNext(t, LPAREN);
    ExpectNext(t, EXP_AND);
    ParseFacts(fact_list, count, t);
    ExpectNext(t, RPAREN);
}

void ProblemParse(Problem *problem, const char *str) {
    LexerInit(str);
    Token t;
    ExpectNext(&t, LPAREN);
    ExpectNext(&t, DEF_DEFINE);
    while (LexerNext(&t)) {
        if (t.kind == RPAREN) break;
        Expect(t.kind, LPAREN);
        LexerNext(&t);
        TRACE("Parsing %s token in problem parsing", TOKEN_NAMES[t.kind]);
        switch (t.kind) {
        case DEF_NAME: ParseName(&problem->name, &t); break;
        case DEF_DOMAIN: ParseName(&problem->domain, &t); break;
        case DEF_OBJECTS: ParseList(problem->objects, &problem->object_count, &t); break;
        case DEF_INIT: ParseFacts(problem->inits, &problem->init_count, &t); break;
        case DEF_GOAL: ParseGoal(problem->goals, &problem->goal_count, &t); break;
        default: ERROR("Unexpected token %s", TOKEN_NAMES[t.kind]); exit(1);
        }
    }

    INFO("Objects: %d", problem.object_count);
    INFO("Inits: %d", problem.init_count);
    INFO("Goals: %d", problem.goal_count);

    if (problem->object_count > MAX_OBJECTS * 0.5)
        WARN("Problem has %d objects, which is near maximum.", problem->object_count);
    if (problem->init_count > MAX_INITS * 0.5)
        WARN("Problem has %d initial facts, which is near maximum.", problem->object_count);
    if (problem->goal_count > MAX_GOALS * 0.5)
        WARN("Problem has %d goal facts, which is near maximum.", problem->object_count);
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
