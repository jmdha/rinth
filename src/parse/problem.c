#include <memory.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "lexer.h"
#include "log.h"
#include "problem.h"

static void ParseName(char **name, Token *t, const char *str) {
    ExpectNext(t, ID);
    WriteToken(name, t, str);
    ExpectNext(t, RPAREN);
}

static void ParseList(char **list, uint *count, Token *t, const char *str) {
    *count = 0;
    while (LexerNext(t)) {
        if (t->kind == ID)
            WriteToken(&list[(*count)++], t, str);
        else if (t->kind == RPAREN)
            return;
        else
            ExpectEither(t->kind, ID, RPAREN);
    }
    EOI(RPAREN);
}

static void ParseFact(SFact *fact, Token *t, const char *str) {
    ExpectNext(t, ID);
    WriteToken(&fact->predicate, t, str);
    ParseList(fact->args, &fact->arg_count, t, str);
}

static void ParseFacts(SFact *fact_list, uint *count, Token *t, const char *str) {
    while (LexerNext(t)) {
        if (t->kind == LPAREN)
            ParseFact(&(fact_list[(*count)++]), t, str);
        else if (t->kind == RPAREN)
            return;
        else
            ExpectEither(t->kind, LPAREN, RPAREN);
    }
    EOI(RPAREN);
}

// TODO: Handle other things than AND
static void ParseGoal(SFact *fact_list, uint *count, Token *t, const char *str) {
    ExpectNext(t, LPAREN);
    ExpectNext(t, EXP_AND);
    ParseFacts(fact_list, count, t, str);
    ExpectNext(t, RPAREN);
}

Problem ProblemParse(const char *str) {
    LexerInit(str);
    Problem problem = {
        .name = NULL, .domain = NULL, .object_count = 0, .init_count = 0, .goal_count = 0
    };
    Token t;

    ExpectNext(&t, LPAREN);
    ExpectNext(&t, DEF_DEFINE);
    while (LexerNext(&t)) {
        if (t.kind == RPAREN) break;
        Expect(t.kind, LPAREN);
        LexerNext(&t);
        TRACE("Parsing %s token in problem parsing", TOKEN_NAMES[t.kind]);
        switch (t.kind) {
        case DEF_NAME: ParseName(&problem.name, &t, str); break;
        case DEF_DOMAIN: ParseName(&problem.domain, &t, str); break;
        case DEF_OBJECTS: ParseList(problem.objects, &problem.object_count, &t, str); break;
        case DEF_INIT: ParseFacts(problem.inits, &problem.init_count, &t, str); break;
        case DEF_GOAL: ParseGoal(problem.goals, &problem.goal_count, &t, str); break;
        default: ERROR("Unexpected token %s", TOKEN_NAMES[t.kind]); exit(1);
        }
    }

    INFO("Objects: %d", problem.object_count);
    INFO("Inits: %d", problem.init_count);
    INFO("Goals: %d", problem.goal_count);

    if (problem.object_count > MAX_OBJECTS * 0.5)
        WARN("Problem has %d objects, which is near maximum.", problem.object_count);
    if (problem.init_count > MAX_INITS * 0.5)
        WARN("Problem has %d initial facts, which is near maximum.", problem.object_count);
    if (problem.goal_count > MAX_GOALS * 0.5)
        WARN("Problem has %d goal facts, which is near maximum.", problem.object_count);

    return problem;
}

void ProblemDelete(Problem *problem) {
    free(problem->domain);
    free(problem->name);
    for (uint i = 0; i < problem->object_count; i++)
        free(problem->objects[i]);
    for (uint i = 0; i < problem->init_count; i++) {
        free(problem->inits[i].predicate);
        for (uint t = 0; t < problem->inits[i].arg_count; t++)
            free(problem->inits[i].args[t]);
    }
    for (uint i = 0; i < problem->goal_count; i++) {
        free(problem->goals[i].predicate);
        for (uint t = 0; t < problem->goals[i].arg_count; t++)
            free(problem->goals[i].args[t]);
    }
}

void ProblemPrint(Problem *problem) {
    printf("Domain: %s\n", problem->domain);
    printf("Name: %s\n", problem->domain);
    printf("Object count: %d\n", problem->object_count);
    printf("Objects:\n");
    for (uint i = 0; i < problem->object_count; i++)
        printf("\t%s\n", problem->objects[i]);
    printf("Init count: %d\n", problem->init_count);
    printf("Inits:\n");
    for (uint i = 0; i < problem->init_count; i++) {
        printf("\t%s", problem->inits[i].predicate);
        for (uint t = 0; t < problem->inits[i].arg_count; t++)
            printf(" %s", problem->inits[i].args[t]);
        printf("\n");
    }
    printf("Goal count: %d\n", problem->goal_count);
    printf("Goals:\n");
    for (uint i = 0; i < problem->goal_count; i++) {
        printf("\t%s", problem->goals[i].predicate);
        for (uint t = 0; t < problem->goals[i].arg_count; t++)
            printf(" %s", problem->goals[i].args[t]);
        printf("\n");
    }
}
