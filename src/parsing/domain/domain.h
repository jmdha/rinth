#pragma once

#define MAX_PARAMETERS 8
#define MAX_PREDICATES 32
#define MAX_ACTIONS 32
#define MAX_EXPRESSION_BRACNHES 16

typedef struct {
    char *name;
    int type;
} Parameter;

typedef struct {
    char *name;
    int parameter_count;
    Parameter parameters[MAX_PARAMETERS];
} Predicate;

typedef struct Expression {
    enum { FACT, EQUAL, AND, OR, NOT } kind;
    union {
        struct {
            int predicate;
            int parameter;
        } FACT;
        struct {
            struct Expression *children[MAX_EXPRESSION_BRACNHES];
        } NARY;
        struct {
            struct Expression *child;
        } UNARY;
    } data;
} Expression;

typedef struct {
    char *name;
    int parameter_count;
    Parameter parameters[MAX_PARAMETERS];
    Expression precondition;
    Expression effect;
} Action;

typedef struct {
    char *name;
    int predicate_count;
    Predicate predicates[MAX_PREDICATES];
    int action_count;
    Action actions[MAX_ACTIONS];
} Domain;
