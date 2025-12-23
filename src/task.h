#ifndef TASK_H
#define TASK_H

#include "misc.h"

#define MAX_PREDICATES 16
#define MAX_VARS       8
#define MAX_ATOMS      32
#define MAX_ACTIONS    64
#define MAX_OBJECTS    1024
#define MAX_INITS      1024
#define MAX_GOALS      128

typedef struct {
	size_t predicate;
	size_t arity;
	size_t vars[MAX_VARS];
} atom;

typedef struct {
	string name;
	size_t arity;
	atom   pre_pos[MAX_ATOMS];
	atom   pre_neg[MAX_ATOMS];
	atom   eff_pos[MAX_ATOMS];
	atom   eff_neg[MAX_ATOMS];
} action;

typedef struct {
	string predicates[MAX_PREDICATES];
	string objects[MAX_OBJECTS];
	action actions[MAX_ACTIONS];
	atom   inits[MAX_INITS];
	atom   goals[MAX_GOALS];
} task;

#endif
