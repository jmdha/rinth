#pragma once

#include "misc.h"

#define MAX_REQUIREMENTS 8
#define MAX_PREDICATE_VARIABLES 4
#define MAX_PREDICATES 16
#define MAX_ACTION_VARIABLES 8
#define MAX_ACTIONS 8
#define MAX_EXPRESSION_BRANCHING 16

typedef struct {
    char *name;
    uint var_count;
    char *vars[MAX_PREDICATE_VARIABLES];
} Predicate;

typedef struct Expression {
    enum { E_ATOM, E_AND, E_NOT, E_OR } kind;
    union {
        struct {
            char *predicate;
            uint var_count;
            char *vars[MAX_PREDICATE_VARIABLES];
        } atom;
        struct Expression *unary;
        struct {
            uint count;
            struct Expression *exps[MAX_EXPRESSION_BRANCHING];
        } nary;
    } data;
} Expression;

typedef struct {
    char *name;
    uint var_count;
    char *vars[MAX_ACTION_VARIABLES];
    Expression *precondition;
    Expression *effect;
} Action;

typedef struct {
    char *name;
    uint requirement_count;
    char *requirements[MAX_REQUIREMENTS];
    uint predicate_count;
    Predicate predicates[MAX_PREDICATES];
    uint action_count;
    Action actions[MAX_ACTIONS];
} Domain;

Domain DomainParse(const char *str);
void DomainDelete(Domain *domain);

void DomainPrint(Domain *domain);
