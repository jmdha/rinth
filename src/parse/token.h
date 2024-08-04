#pragma once

typedef enum {
    DEF_DEFINE,
    DEF_NAME,
    DEF_REQUIREMENTS,
    DEF_PREDICATES,
    DEF_ACTION,
    DEF_PARAMETERS,
    DEF_PRECONDITION,
    DEF_EFFECT,
    DEF_DOMAIN,
    DEF_OBJECTS,
    DEF_INIT,
    DEF_GOAL,
    REQ_STRIPS,
    EXP_AND,
    EXP_NOT,
    EXP_OR,
    LPAREN,
    RPAREN,
    VARIABLE,
    ID,
    MAX_TOKEN_KIND
} TokenKind;

static const char *TOKEN_NAMES[MAX_TOKEN_KIND] = {
    "DEF_DEFINE",     "DEF_NAME",         "DEF_REQUIREMENTS", "DEF_PREDICATES", "DEF_ACTION",
    "DEF_PARAMETERS", "DEF_PRECONDITION", "DEF_EFFECT",       "DEF_DOMAIN",     "DEF_OBJECTS",
    "DEF_INIT",       "DEF_GOAL",         "REQ_STRIPS",       "EXP_AND",        "EXP_NOT",
    "EXP_OR",         "LPAREN",           "RPAREN",           "VARIABLE",       "ID",
};
