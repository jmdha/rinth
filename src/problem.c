#include <memory.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "lexer.h"
#include "log.h"
#include "problem.h"

static inline void ParseName(char **name, Token *t, Lexer *lexer, const char *str) {
    ExpectNext(t, lexer, ID);
    WriteToken(name, t, str);
    ExpectNext(t, lexer, RPAREN);
}

static inline void ParseList(char **list, int *count, Token *t, Lexer *lexer, const char *str) {
    *count = 0;
    while (LexerNext(t, lexer)) {
        if (t->kind == ID)
            WriteToken(&list[(*count)++], t, str);
        else if (t->kind == RPAREN)
            return;
        else
            ExpectEither(t->kind, ID, RPAREN);
    }
    EOI(RPAREN);
}

static inline void ParseFact(Fact *fact, Token *t, Lexer *lexer, const char *str) {
    ExpectNext(t, lexer, ID);
    WriteToken(&fact->predicate, t, str);
    ParseList(fact->args, &fact->arg_count, t, lexer, str);
}

static inline void
ParseFacts(Fact *fact_list, int *count, Token *t, Lexer *lexer, const char *str) {
    while (LexerNext(t, lexer)) {
        if (t->kind == LPAREN)
            ParseFact(&(fact_list[(*count)++]), t, lexer, str);
        else if (t->kind == RPAREN)
            return;
        else
            ExpectEither(t->kind, LPAREN, RPAREN);
    }
    EOI(RPAREN);
}

// TODO: Handle other things than AND
static inline void ParseGoal(Fact *fact_list, int *count, Token *t, Lexer *lexer, const char *str) {
    ExpectNext(t, lexer, LPAREN);
    ExpectNext(t, lexer, EXP_AND);
    ParseFacts(fact_list, count, t, lexer, str);
    ExpectNext(t, lexer, RPAREN);
}

Problem ProblemParse(const char *str) {
    Problem problem = {
        .name = NULL, .domain = NULL, .object_count = 0, .init_count = 0, .goal_count = 0
    };
    Lexer lexer = {.str = str, .pos = 0};
    Token t;

    ExpectNext(&t, &lexer, LPAREN);
    ExpectNext(&t, &lexer, DEF_DEFINE);
    while (LexerNext(&t, &lexer)) {
        if (t.kind == RPAREN) break;
        Expect(t.kind, LPAREN);
        LexerNext(&t, &lexer);
        TRACE("Parsing %s token in problem parsing", TOKEN_NAMES[t.kind]);
        switch (t.kind) {
        case DEF_NAME: ParseName(&problem.name, &t, &lexer, str); break;
        case DEF_DOMAIN: ParseName(&problem.domain, &t, &lexer, str); break;
        case DEF_OBJECTS: ParseList(problem.objects, &problem.object_count, &t, &lexer, str); break;
        case DEF_INIT: ParseFacts(problem.inits, &problem.init_count, &t, &lexer, str); break;
        case DEF_GOAL: ParseGoal(problem.goals, &problem.goal_count, &t, &lexer, str); break;
        default: ERROR("Unexpected token %s", TOKEN_NAMES[t.kind]); exit(1);
        }
    }

    DEBUG("Objects: %d", problem.object_count);
    DEBUG("Inits: %d", problem.init_count);
    DEBUG("Goals: %d", problem.goal_count);

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
    for (int i = 0; i < problem->object_count; i++)
        free(problem->objects[i]);
    for (int i = 0; i < problem->init_count; i++) {
        free(problem->inits[i].predicate);
        for (int t = 0; t < problem->inits[i].arg_count; t++)
            free(problem->inits[i].args[t]);
    }
    for (int i = 0; i < problem->goal_count; i++) {
        free(problem->goals[i].predicate);
        for (int t = 0; t < problem->goals[i].arg_count; t++)
            free(problem->goals[i].args[t]);
    }
}

void ProblemPrint(Problem *problem) {
    printf("Domain: %s\n", problem->domain);
    printf("Name: %s\n", problem->domain);
    printf("Object count: %d\n", problem->object_count);
    printf("Objects:\n");
    for (int i = 0; i < problem->object_count; i++)
        printf("\t%s\n", problem->objects[i]);
    printf("Init count: %d\n", problem->init_count);
    printf("Inits:\n");
    for (int i = 0; i < problem->init_count; i++) {
        printf("\t%s", problem->inits[i].predicate);
        for (int t = 0; t < problem->inits[i].arg_count; t++)
            printf(" %s", problem->inits[i].args[t]);
        printf("\n");
    }
    printf("Goal count: %d\n", problem->goal_count);
    printf("Goals:\n");
    for (int i = 0; i < problem->goal_count; i++) {
        printf("\t%s", problem->goals[i].predicate);
        for (int t = 0; t < problem->goals[i].arg_count; t++)
            printf(" %s", problem->goals[i].args[t]);
        printf("\n");
    }
}
