#include "token.h"

Token TokenCreate(TokenKind kind, int pos, int len) {
    Token token;
    token.kind = kind;
    token.pos  = pos;
    token.len  = len;
    return token;
}

const char *TOKEN_NAME[MAX_TOKEN] = {
    "TOKEN_EOF",
    "TOKEN_NONE",
    "TOKEN_WHITESPACE",
    "TOKEN_LPAREN",
    "TOKEN_RPAREN",
    "TOKEN_ID",
    "TOKEN_DEF_DOMAIN",
    "TOKEN_DEF_PREDICATE",
    "TOKEN_DEF_ACTION",
    "TOKEN_DEF_PARAMETERS",
    "TOKEN_DEF_PRECONDITION",
    "TOKEN_DEF_EFFECT",
};
