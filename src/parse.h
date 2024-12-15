#pragma once

#include "bounds.h"
#include "misc.h"

struct predicate {
    string name;
    uint   var_count;
    string vars[MAX_VARIABLES];
};

struct expression {
    enum { E_ATOM, E_NOT, E_AND, E_OR } kind;
    union {
        struct {
            string predicate;
            uint   arg_count;
            string args[MAX_VARIABLES];
        } atom;
        struct {
            uint               count;
            struct expression* exps[MAX_BRANCHING];
        } nary;
        struct expression* unary;
    };
};

struct action {
           string      name;
           uint        var_count;
           string      vars[MAX_VARIABLES];
    struct expression* precondition;
    struct expression* effect;
};

struct domain {
           string    name;
           uint      requirement_count;
           string    requirements[MAX_REQUIREMENTS];
           uint      predicate_count;
    struct predicate predicates[MAX_PREDICATES];
           uint      action_count;
    struct action    actions[MAX_ACTIONS];
};

struct fact {
    string predicate;
    uint   arg_count;
    string args[MAX_VARIABLES];
};

struct problem {
           string domain;
           string name;
           uint   object_count;
           string objects[MAX_OBJECTS];
           uint   init_count;
    struct fact   inits[MAX_INITS];
           uint   goal_count;
    struct fact   goals[MAX_GOALS];
};

void           parse_domain_(struct domain*, const char*);
void           parse_problem_(struct problem*, const char*);
struct domain  parse_domain(const char*);
struct problem parse_problem(const char*);
