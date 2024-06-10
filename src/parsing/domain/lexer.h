#pragma once

#include "token.h"

typedef struct {
    int pos;
} Lexer;

Lexer LexerInit(void);
Token LexerNext(Lexer *lexer, const char *str);
Token LexerFirst(const char *str);
