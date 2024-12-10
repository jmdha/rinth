#pragma once

#include "misc.h"
#include "bounds.h"
#include "parse/domain.h"
#include "parse/problem.h"
#include "fact.h"

struct task {
    string      domain_name;                    // Name of domain
    string      problem_name;                   // Name of problem
    uint        predicate_count;                // How many predicates exist
    uint        predicate_vars[MAX_PREDICATES]; // How many variables each predicate has
    string      predicates[MAX_PREDICATES];     // The name of each variable
    uint        object_count;                   // How many objects exist
    string      objects[MAX_OBJECTS];           // The name of each object
    uint        init_count;                     // How many facts the initial state contains
    struct fact inits[MAX_INITS];               // Facts of the initial state
    uint        goal_count;                     // How many facts the goal state contains
    struct fact goals[MAX_INITS];               // Facts of the goal state
};

struct task translate(const Domain* domain, const Problem* problem);
