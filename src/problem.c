#include <ctype.h>
#include <memory.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "log.h"
#include "problem.h"

typedef enum {
    DEF_DEFINE,
    DEF_NAME,
    DEF_DOMAIN,
    DEF_OBJECTS,
    DEF_INIT,
    DEF_GOAL,
    EXP_AND,
    EXP_NOT,
    EXP_OR,
    EXP_EQ,
    LPAREN,
    RPAREN,
    ID,
    MAX_TOKEN_KIND
} TokenKind;

const char *TOKEN_NAMES[MAX_TOKEN_KIND] = {
    "DEF_DEFINE", "DEF_NAME", "DEF_DOMAIN", "DEF_OBJECTS", "DEF_INIT", "DEF_GOAL", "EXP_AND",
    "EXP_NOT",    "EXP_OR",   "EXP_EQ",     "LPAREN",      "RPAREN",   "ID",

};

typedef struct {
    TokenKind kind;
    int pos, len;
} Token;

void TokenAssign(Token *token, TokenKind kind, int pos, int len) {
    token->kind = kind;
    token->pos  = pos;
    token->len  = len;
}

typedef struct {
    const char *str;
    int pos;
} Lexer;

bool LexID(Lexer *lexer) {
    const int pos = lexer->pos;

    while (isalpha(lexer->str[lexer->pos]) || isdigit(lexer->str[lexer->pos]) ||
           lexer->str[lexer->pos] == '-' || lexer->str[lexer->pos] == '_')
        lexer->pos++;

    return pos != lexer->pos;
}

bool MatchDef(TokenKind *kind, const char *str, int len) {
    if (len == 2 && strncasecmp("or", str, len) == 0)
        *kind = EXP_OR;
    else if (len == 3 && strncasecmp("and", str, len) == 0)
        *kind = EXP_AND;
    else if (len == 3 && strncasecmp("not", str, len) == 0)
        *kind = EXP_NOT;
    else if (len == 6 && strncasecmp("define", str, len) == 0)
        *kind = DEF_DEFINE;
    else if (len == 7 && strncasecmp("problem", str, len) == 0)
        *kind = DEF_NAME;
    else if (len == 7 && strncasecmp(":domain", str, len) == 0)
        *kind = DEF_DOMAIN;
    else if (len == 8 && strncasecmp(":objects", str, len) == 0)
        *kind = DEF_OBJECTS;
    else if (len == 5 && strncasecmp(":init", str, len) == 0)
        *kind = DEF_INIT;
    else if (len == 5 && strncasecmp(":goal", str, len) == 0)
        *kind = DEF_GOAL;
    else
        return false;
    return true;
}

bool LexerNext(Token *token, Lexer *lexer) {
    const int pos = lexer->pos++;
    const char c  = lexer->str[pos];

    if (isspace(c)) return LexerNext(token, lexer);

    switch (c) {
    case '\0': return false;
    case '(': TokenAssign(token, LPAREN, pos, 1); break;
    case ')': TokenAssign(token, RPAREN, pos, 1); break;
    case '=': TokenAssign(token, EXP_EQ, pos, 1); break;
    case ':': {
        if (!LexID(lexer)) {
            ERROR("Found errant ':' at position %d", pos);
            exit(1);
        }
        const int len = lexer->pos - pos;
        if (!MatchDef(&token->kind, &lexer->str[pos], len)) {
            ERROR("Unknown definition \"%.*s\" found at position %d", len, &lexer->str[pos], pos);
            exit(1);
        }
        token->pos = pos;
        token->len = len;
        break;
    }
    default: {
        token->kind = ID;
        if (!LexID(lexer)) {
            ERROR("Unknown character '%c' at position %d", c, pos);
            exit(1);
        }
        const int len = lexer->pos - pos;
        MatchDef(&token->kind, &lexer->str[pos], len);
        token->pos = pos;
        token->len = len;
        break;
    }
    }

    return true;
}

void EOI(TokenKind expected) {
    ERROR("Unexpected end of input. Expected %s", TOKEN_NAMES[expected]);
    exit(1);
}

void Expect(TokenKind actual, TokenKind expected) {
    if (actual != expected) {
        ERROR(
            "Found unexpected token %s while expecting %s", TOKEN_NAMES[actual],
            TOKEN_NAMES[expected]
        );
        exit(1);
    }
}

void ExpectEither(TokenKind actual, TokenKind e1, TokenKind e2) {
    if (actual != e1 && actual != e2) {
        ERROR(
            "Found unexpected token %s while expecting %s or %s", TOKEN_NAMES[actual],
            TOKEN_NAMES[e1], TOKEN_NAMES[e2]
        );
        exit(1);
    }
}

void ExpectNext(Token *t, Lexer *lexer, TokenKind kind) {
    if (!LexerNext(t, lexer)) EOI(kind);
    Expect(t->kind, kind);
}

void WriteID(char **id, Token *t, const char *str) {
    *id           = malloc(t->len + 1);
    *id           = strncpy(*id, &str[t->pos], t->len);
    (*id)[t->len] = '\0';
}

void ParseName(char **name, Token *t, Lexer *lexer, const char *str) {
    ExpectNext(t, lexer, ID);
    WriteID(name, t, str);
    ExpectNext(t, lexer, RPAREN);
}

void ParseList(char **list, int *count, Token *t, Lexer *lexer, const char *str) {
    while (LexerNext(t, lexer)) {
        if (t->kind == ID)
            WriteID(&list[(*count)++], t, str);
        else if (t->kind == RPAREN)
            return;
        else
            ExpectEither(t->kind, ID, RPAREN);
    }
    EOI(RPAREN);
}

void ParseFact(Fact *fact, Token *t, Lexer *lexer, const char *str) {
    ExpectNext(t, lexer, ID);
    WriteID(&fact->predicate, t, str);
    ParseList(fact->args, &fact->arg_count, t, lexer, str);
}

void ParseFacts(Fact *fact_list, int *count, Token *t, Lexer *lexer, const char *str) {
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
void ParseGoal(Fact *fact_list, int *count, Token *t, Lexer *lexer, const char *str) {
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
