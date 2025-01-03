#pragma once

#include "misc.h"
#include "bounds.h"
#include "parse.h"
#include "state.h"

typedef struct {
    uint predicate;
    uint arg_count;
    uint args[MAX_VARIABLES];
    bool val;
} Atom;

typedef struct {
    string name;
    uint vars;
    uint pre_count;
    Atom pre[MAX_ATOMS];
    uint eff_count;
    Atom eff[MAX_ATOMS];
} Scheme;

struct task {
    string domain_name;                    // Name of domain
    string problem_name;                   // Name of problem
    uint   predicate_count;                // How many predicates exist
    uint   predicate_vars[MAX_PREDICATES]; // How many variables each predicate has
    string predicates[MAX_PREDICATES];     // The name of each predicate
    uint   object_count;                   // How many objects exist
    string objects[MAX_OBJECTS];           // The name of each object
    state* init;                           // The initial state
    state* goal;                           // The goal state
    uint   scheme_count;                   // How many schemes exist
    Scheme schemes[MAX_SCHEMES];
};

struct task translate(const struct domain*, const struct problem*);
void task_free(struct task*);
