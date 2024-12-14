#pragma once

#include <stdbool.h>

#include "misc.h"

enum kind { 
    KIND_EOI,
    KIND_LPAREN,
    KIND_RPAREN,
    KIND_ID,
    MAX_KIND
};

enum keyword {
    KEYWORD_DEFINE,
    KEYWORD_NAME,
    KEYWORD_DOMAIN,
    KEYWORD_REQUIREMENTS,
    KEYWORD_PREDICATES,
    KEYWORD_ACTION,
    KEYWORD_PARAMETERS,
    KEYWORD_PRECONDITION,
    KEYWORD_EFFECT,
    KEYWORD_AND,
    KEYWORD_OR,
    KEYWORD_NOT,
    KEYWORD_OBJECTS,
    KEYWORD_INIT,
    KEYWORD_GOAL,
    MAX_KEYWORD
};

extern const char *KIND_NAMES[MAX_KIND + 1];
extern const char *KEYWORD_NAMES[MAX_KEYWORD + 1];

void      lexer_init(const char *str);       // Initialise lexer
enum kind lexer_next(string *str);           // Find next token in stream
void      lexer_expect(enum kind kind);      // Execute lexer_next and abort if not correct token kind
void      lexer_expect_def(enum keyword kw); // Execute lexer_next and abort if not correct token kind

enum keyword keyword_match(const string *str); // Matches string to keyword
