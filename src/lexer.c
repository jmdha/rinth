#include "lexer.h"
#include "log.h"
#include "token.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

Lexer LexerInit(void) {
    Lexer lexer;
    lexer.pos = 0;
    return lexer;
}

Token LexerNext(Lexer *lexer, const char *str) {
    switch (str[lexer->pos]) {
    case '\0': return TokenCreate(TOKEN_EOF, lexer->pos++, 1);
    case '(': return TokenCreate(TOKEN_LPAREN, lexer->pos++, 1);
    case ')': return TokenCreate(TOKEN_RPAREN, lexer->pos++, 1);
    }
    if (isspace(str[lexer->pos])) {
        const int start = lexer->pos;

        while (isspace(str[lexer->pos])) {
            lexer->pos++;
        }

        return TokenCreate(TOKEN_WHITESPACE, start, lexer->pos - start);
    }
    if (str[lexer->pos] == ':') {
        const int start = lexer->pos;

        while (!isspace(str[lexer->pos]) && str[lexer->pos] != '\0') {
            lexer->pos++;
        }

        const int len = lexer->pos - start;
        if (len == 7 && strncmp(&str[start], ":action", len) == 0)
            return TokenCreate(TOKEN_DEF_ACTION, start, len);

        ERROR("Lexer failed to identify '%.*s' at position %d.", len, &str[start], start);
        exit(1);
    }
    ERROR("Lexer failed to identify '%c' at position %d.", str[lexer->pos], lexer->pos);
    exit(1);
}
