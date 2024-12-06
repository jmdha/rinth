#pragma once

#include "misc.h"

#define MAX_REQUIREMENTS         8
#define MAX_PREDICATE_VARIABLES  4
#define MAX_PREDICATES           16
#define MAX_ACTION_VARIABLES     8
#define MAX_ACTIONS              8
#define MAX_EXPRESSION_BRANCHING 16

typedef struct {
    string name;
    uint   var_count;
    string vars[MAX_PREDICATE_VARIABLES];
} Predicate;

typedef struct Expression {
    enum { E_ATOM, E_AND, E_NOT, E_OR } kind;
    union {
        struct {
            string predicate;
            uint   var_count;
            string vars[MAX_PREDICATE_VARIABLES];
        } atom;
        struct Expression *unary;
        struct {
            uint count;
            struct Expression *exps[MAX_EXPRESSION_BRANCHING];
        } nary;
    } data;
} Expression;

typedef struct {
    string name;
    uint   var_count;
    string  vars[MAX_ACTION_VARIABLES];
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

void DomainParse(Domain *domain, const char *str);

uint DomainPredicateIndex(const Domain *domain, const string *str);
uint ActionVarIndex(const Action *action, const string *str);
