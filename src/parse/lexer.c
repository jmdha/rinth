#include <ctype.h>
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
    token->str.ptr = &STR[pos];
    token->str.len = len;
}

void LexID() {
    while (isalpha(STR[POS]) || isdigit(STR[POS]) || STR[POS] == '-' || STR[POS] == '_')
        POS++;
}

bool MatchDef(TokenKind *kind, const char *str, uint len) {
    if (len == 13 && strncmp(":requirements", str, len) == 0)
        *kind = DEF_REQUIREMENTS;
    else if (len == 11 && strncmp(":predicates", str, len) == 0)
        *kind = DEF_PREDICATES;
    else if (len == 7 && strncmp(":action", str, len) == 0)
        *kind = DEF_ACTION;
    else if (len == 11 && strncmp(":parameters", str, len) == 0)
        *kind = DEF_PARAMETERS;
    else if (len == 13 && strncmp(":precondition", str, len) == 0)
        *kind = DEF_PRECONDITION;
    else if (len == 7 && strncmp(":effect", str, len) == 0)
        *kind = DEF_EFFECT;
    else if (len == 7 && strncmp(":domain", str, len) == 0)
        *kind = DEF_DOMAIN;
    else if (len == 8 && strncmp(":objects", str, len) == 0)
        *kind = DEF_OBJECTS;
    else if (len == 5 && strncmp(":init", str, len) == 0)
        *kind = DEF_INIT;
    else if (len == 5 && strncmp(":goal", str, len) == 0)
        *kind = DEF_GOAL;
    else if (len == 7 && strncmp(":strips", str, len) == 0)
        *kind = REQ_STRIPS;
    return true;
}

bool MatchKeyword(TokenKind *kind, const char *str, uint len) {
    if (len == 6 && strncmp("define", str, len) == 0)
        *kind = DEF_DEFINE;
    else if (len == 6 && strncmp("domain", str, len) == 0)
        *kind = DEF_NAME;
    else if (len == 7 && strncmp("problem", str, len) == 0)
        *kind = DEF_NAME;
    else if (len == 3 && strncmp("and", str, len) == 0)
        *kind = EXP_AND;
    else if (len == 3 && strncmp("not", str, len) == 0)
        *kind = EXP_NOT;
    else if (len == 2 && strncmp("or", str, len) == 0)
        *kind = EXP_OR;
    else
        return false;
    return true;
}

bool LexerNext(Token *token) {
    int pos;
    char c;
    while ((c = STR[pos = POS++]) < 33) {} 

    switch (c) {
    case '\0': return false;
    case '(': TokenAssign(token, LPAREN, pos, 1); break;
    case ')': TokenAssign(token, RPAREN, pos, 1); break;
    case ':': {
        LexID();
        const int len = POS - pos;
        if (!MatchDef(&token->kind, &STR[pos], len))
            ERROR("Unknown definition \"%.*s\" found at position %d", len, &STR[pos], pos);
        token->str.ptr = &STR[pos];
        token->str.len = len;
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
        token->str.ptr = &STR[pos];
        token->str.len = len;
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
