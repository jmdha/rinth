#pragma once

#include "misc.h"
#include "bounds.h"

typedef struct {
    string name;
    uint   var_count;
    string vars[MAX_VARIABLES];
} Predicate;

typedef struct Expression {
    enum { E_ATOM, E_AND, E_NOT, E_OR } kind;
    union {
        struct {
            string predicate;
            uint   var_count;
            string vars[MAX_VARIABLES];
        } atom;
        struct Expression *unary;
        struct {
            uint count;
            struct Expression *exps[MAX_BRANCHING];
        } nary;
    } data;
} Expression;

typedef struct {
    string name;
    uint   var_count;
    string  vars[MAX_VARIABLES];
    Expression *precondition;
    Expression *effect;
} Action;

typedef struct {
    string    name;
    uint      requirement_count;
    string    requirements[MAX_REQUIREMENTS];
    uint      predicate_count;
    Predicate predicates[MAX_PREDICATES];
    uint      action_count;
    Action    actions[MAX_ACTIONS];
} Domain;

Domain domain_parse(const char *str);
