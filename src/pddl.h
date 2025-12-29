#ifndef PDDL_H
#define PDDL_H

#include <stdbool.h>

#include "misc.h"

#define MAX_PDDL_REQUIREMENTS 4
#define MAX_PDDL_PREDICATES   8
#define MAX_PDDL_VARS         4
#define MAX_PDDL_ACTIONS      8
#define MAX_PDDL_STACK        512
#define MAX_PDDL_OBJECTS      128
#define MAX_PDDL_INITS        512
#define MAX_PDDL_GOALS        128

typedef struct {
	string      name;
	string      vars[MAX_PDDL_VARS];
} pddl_pred;

typedef struct {
	string      name;
	string      vars[MAX_PDDL_VARS];
	string      pre[MAX_PDDL_STACK];
	string      eff[MAX_PDDL_STACK];
} pddl_action;

typedef struct {
	string      name;
	string      requirements[MAX_PDDL_REQUIREMENTS];
	pddl_pred   predicates[MAX_PDDL_PREDICATES];
	pddl_action actions[MAX_PDDL_ACTIONS];
} pddl_domain;

typedef struct {
	string      predicate;
	string      vars[MAX_PDDL_VARS];
} pddl_atom;

typedef struct {
	string      name;
	string      domain;
	string      objects[MAX_PDDL_OBJECTS];
	pddl_atom   inits[MAX_PDDL_INITS];
	pddl_atom   goals[MAX_PDDL_GOALS];
} pddl_problem;

pddl_domain  pddl_domain_parse(const char* s);
pddl_problem pddl_problem_parse(const char* s);
bool         pddl_domain_validate(const pddl_domain* d);
bool         pddl_problem_validate(const pddl_domain* d, const pddl_problem* p);
bool         pddl_validate(const pddl_domain* d, const pddl_problem* p);

#endif
