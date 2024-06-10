#include "lexer.h"
#include "token.h"
#include <ctype.h>
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
        if (len == 11 && strncmp(&str[start], ":predicates", len) == 0)
            return TokenCreate(TOKEN_DEF_PREDICATE, start, len);
        if (len == 7 && strncmp(&str[start], ":action", len) == 0)
            return TokenCreate(TOKEN_DEF_ACTION, start, len);
        if (len == 11 && strncmp(&str[start], ":parameters", len) == 0)
            return TokenCreate(TOKEN_DEF_PARAMETERS, start, len);
        if (len == 13 && strncmp(&str[start], ":precondition", len) == 0)
            return TokenCreate(TOKEN_DEF_PRECONDITION, start, len);
        if (len == 7 && strncmp(&str[start], ":effect", len) == 0)
            return TokenCreate(TOKEN_DEF_EFFECT, start, len);

        return TokenCreate(TOKEN_UNKNOWN, start, len);
    }
    if (isalpha(str[lexer->pos])) {
        const int start = lexer->pos;

        while (isalpha(str[lexer->pos]) || isdigit(str[lexer->pos]) || str[lexer->pos] == '-' ||
               str[lexer->pos] == '_' || str[lexer->pos] == '.') {
            lexer->pos++;
        }

        return TokenCreate(TOKEN_ID, start, lexer->pos - start);
    }
    return TokenCreate(TOKEN_UNKNOWN, lexer->pos, 1);
}

Token LexerFirst(const char *str) {
    Lexer lexer = LexerInit();
    return LexerNext(&lexer, str);
}
