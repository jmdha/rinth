#pragma once

#include <stdbool.h>

#include "token.h"
#include "misc.h"

typedef struct {
    TokenKind kind;
    string str;
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
