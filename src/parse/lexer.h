#pragma once

#include <stdbool.h>

#include "misc.h"

typedef enum {
    DEF_DEFINE,
    DEF_NAME,
    DEF_REQUIREMENTS,
    DEF_PREDICATES,
    DEF_ACTION,
    DEF_PARAMETERS,
    DEF_PRECONDITION,
    DEF_EFFECT,
    DEF_DOMAIN,
    DEF_OBJECTS,
    DEF_INIT,
    DEF_GOAL,
    REQ_STRIPS,
    EXP_AND,
    EXP_NOT,
    EXP_OR,
    LPAREN,
    RPAREN,
    VARIABLE,
    ID,
    MAX_TOKEN_KIND
} TokenKind;

extern const char *TOKEN_NAMES[MAX_TOKEN_KIND];

typedef struct {
    TokenKind kind;
    uint pos, len;
} Token;

void LexerInit(const char *str);

void TokenAssign(Token *token, TokenKind kind, uint pos, uint len);

bool MatchDef(TokenKind *kind, const char *str, uint len);
void LexID();
bool LexerNext(Token *token);

void EOI(TokenKind expected);
void Expect(TokenKind actual, TokenKind expected);
void ExpectEither(TokenKind actual, TokenKind e1, TokenKind e2);
void ExpectNext(Token *t, TokenKind kind);
void Expected(const char *str, TokenKind found);
void WriteToken(char **id, Token *t, const char *str);
