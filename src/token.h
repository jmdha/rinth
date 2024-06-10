#pragma once

typedef enum {
    TOKEN_UNKNOWN,
    TOKEN_EOF,
    TOKEN_NONE,
    TOKEN_WHITESPACE,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_ID,
    TOKEN_DEF_DOMAIN,
    TOKEN_DEF_PREDICATE,
    TOKEN_DEF_ACTION,
    TOKEN_DEF_PARAMETERS,
    TOKEN_DEF_PRECONDITION,
    TOKEN_DEF_EFFECT,
    MAX_TOKEN
} TokenKind;

extern const char *TOKEN_NAME[MAX_TOKEN];

typedef struct {
    TokenKind kind;
    int pos, len;
} Token;

Token TokenCreate(TokenKind kind, int pos, int len);
