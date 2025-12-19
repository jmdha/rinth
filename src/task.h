#pragma once

#include "misc.h"
#include "bounds.h"
#include "state.h"

typedef struct {
	uint predicate;
	uint arg_count;
	uint args[MAX_VARIABLES];
	bool val;
} atom_t;

typedef struct {
	string name;
	uint   vars;
	uint   pre_count;
	atom_t pre[MAX_ATOMS];
	uint   eff_count;
	atom_t eff[MAX_ATOMS];
} schema_t;

typedef struct {
	string   domain_name;                    // Name of domain
	string   problem_name;                   // Name of problem
	uint     predicate_count;                // How many predicates exist
	uint     predicate_vars[MAX_PREDICATES]; // How many variables each predicate has
	string   predicates[MAX_PREDICATES];     // The name of each predicate
	uint     object_count;                   // How many objects exist
	string   objects[MAX_OBJECTS];           // The name of each object
	state*   init;                           // The initial state
	state*   goal;                           // The goal state
	uint     schema_count;                   // How many schemes exist
	schema_t schemas[MAX_SCHEMES];
} task_t;

void task_free(
	task_t*
);
