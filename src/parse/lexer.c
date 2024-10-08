#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "lexer.h"
#include "log.h"

const char *STR;
uint POS;

void LexerInit(const char *str) {
    STR = str;
    POS = 0;
}

void TokenAssign(Token *token, TokenKind kind, uint pos, uint len) {
    token->kind = kind;
    token->pos  = pos;
    token->len  = len;
}

void LexID() {
    while (isalpha(STR[POS]) || isdigit(STR[POS]) || STR[POS] == '-' || STR[POS] == '_')
        POS++;
}

bool MatchDef(TokenKind *kind, const char *str, uint len) {
    if (len == 13 && strncasecmp(":requirements", str, len) == 0)
        *kind = DEF_REQUIREMENTS;
    else if (len == 11 && strncasecmp(":predicates", str, len) == 0)
        *kind = DEF_PREDICATES;
    else if (len == 7 && strncasecmp(":action", str, len) == 0)
        *kind = DEF_ACTION;
    else if (len == 11 && strncasecmp(":parameters", str, len) == 0)
        *kind = DEF_PARAMETERS;
    else if (len == 13 && strncasecmp(":precondition", str, len) == 0)
        *kind = DEF_PRECONDITION;
    else if (len == 7 && strncasecmp(":effect", str, len) == 0)
        *kind = DEF_EFFECT;
    else if (len == 7 && strncasecmp(":domain", str, len) == 0)
        *kind = DEF_DOMAIN;
    else if (len == 8 && strncasecmp(":objects", str, len) == 0)
        *kind = DEF_OBJECTS;
    else if (len == 5 && strncasecmp(":init", str, len) == 0)
        *kind = DEF_INIT;
    else if (len == 5 && strncasecmp(":goal", str, len) == 0)
        *kind = DEF_GOAL;
    else if (len == 7 && strncasecmp(":strips", str, len) == 0)
        *kind = REQ_STRIPS;
    return true;
}

bool MatchKeyword(TokenKind *kind, const char *str, uint len) {
    if (len == 6 && strncasecmp("define", str, len) == 0)
        *kind = DEF_DEFINE;
    else if (len == 6 && strncasecmp("domain", str, len) == 0)
        *kind = DEF_NAME;
    else if (len == 7 && strncasecmp("problem", str, len) == 0)
        *kind = DEF_NAME;
    else if (len == 3 && strncasecmp("and", str, len) == 0)
        *kind = EXP_AND;
    else if (len == 3 && strncasecmp("not", str, len) == 0)
        *kind = EXP_NOT;
    else if (len == 2 && strncasecmp("or", str, len) == 0)
        *kind = EXP_OR;
    else
        return false;
    return true;
}

bool LexerNext(Token *token) {
    const int pos = POS++;
    const char c  = STR[pos];

    if (isspace(c)) return LexerNext(token);

    switch (c) {
    case '\0': return false;
    case '(': TokenAssign(token, LPAREN, pos, 1); break;
    case ')': TokenAssign(token, RPAREN, pos, 1); break;
    case ':': {
        LexID();
        const int len = POS - pos;
        if (!MatchDef(&token->kind, &STR[pos], len))
            ERROR("Unknown definition \"%.*s\" found at position %d", len, &STR[pos], pos);
        token->pos = pos;
        token->len = len;
        break;
    }
    default: {
        token->kind = ID;
        if (c == '?')
            token->kind = VARIABLE;
        else if (!isalpha(c))
            ERROR("Unknown character '%c' at position %d", c, pos);

        LexID();
        const int len = POS - pos;
        MatchKeyword(&token->kind, &STR[pos], len);
        token->pos = pos;
        token->len = len;
        break;
    }
    }

    return true;
}

void EOI(TokenKind expected) { ERROR("Unexpected end of input. Expected %s", TOKEN_NAMES[expected]); }

void Expect(TokenKind actual, TokenKind expected) {
    if (actual != expected)
        ERROR("Found unexpected token %s while expecting %s", TOKEN_NAMES[actual], TOKEN_NAMES[expected]);
}

void ExpectEither(TokenKind actual, TokenKind e1, TokenKind e2) {
    if (actual != e1 && actual != e2) {
        ERROR(
            "Found unexpected token %s while expecting %s or %s", TOKEN_NAMES[actual], TOKEN_NAMES[e1], TOKEN_NAMES[e2]
        );
    }
}

void ExpectNext(Token *t, TokenKind kind) {
    if (!LexerNext(t)) EOI(kind);
    Expect(t->kind, kind);
}

void Expected(const char *str, TokenKind found) { ERROR("Expected %s found %s", str, TOKEN_NAMES[found]); }

void WriteToken(char **id, Token *t, const char *str) {
    *id           = malloc(t->len + 1);
    *id           = strncpy(*id, &str[t->pos], t->len);
    (*id)[t->len] = '\0';
}
