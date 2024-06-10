#include "token.h"

Token TokenCreate(TokenKind kind, int pos, int len) {
    Token token;
    token.kind = kind;
    token.pos  = pos;
    token.len  = len;
    return token;
}

const char *TOKEN_NAME[MAX_TOKEN] = {
    "EOF",        "NONE",          "WHITESPACE", "LPAREN",         "RPAREN",           "ID",
    "DEF_DOMAIN", "DEF_PREDICATE", "DEF_ACTION", "DEF_PARAMETERS", "DEF_PRECONDITION", "DEF_EFFECT",
};
