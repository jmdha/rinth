#pragma once

#include "bounds.h"
#include "misc.h"

struct predicate {
        string name;
        uint var_count;
        string var_types[MAX_VARIABLES];
        string vars[MAX_VARIABLES];
};

struct expression {
        enum { E_ATOM, E_NOT, E_AND, E_OR } kind;
        union {
                struct {
                        string predicate;
                        uint arg_count;
                        string args[MAX_VARIABLES];
                } atom;
                struct {
                        uint count;
                        struct expression *exps[MAX_BRANCHING];
                } nary;
                struct expression *unary;
        };
};

struct action {
        string name;
        uint var_count;
        string var_types[MAX_VARIABLES];
        string vars[MAX_VARIABLES];
        struct expression *precondition;
        struct expression *effect;
};

struct domain {
        string name;
        uint requirement_count;
        string requirements[MAX_REQUIREMENTS];
        uint type_count;
        string type_parents[MAX_TYPES];
        string types[MAX_TYPES];
        uint predicate_count;
        struct predicate predicates[MAX_PREDICATES];
        uint action_count;
        struct action actions[MAX_ACTIONS];
};

struct fact {
        string predicate;
        uint arg_count;
        string args[MAX_VARIABLES];
};

struct problem {
        string domain;
        string name;
        uint object_count;
        string object_types[MAX_OBJECTS];
        string objects[MAX_OBJECTS];
        uint init_count;
        struct fact inits[MAX_INITS];
        uint goal_count;
        struct fact goals[MAX_GOALS];
};

void parse_domain_(struct domain *domain, const char *str);

void parse_problem_(struct problem *problem, const char *str);

struct domain parse_domain(const char *str);

struct problem parse_problem(const char *str);

void domain_free(struct domain *domain);
