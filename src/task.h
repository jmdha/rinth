#ifndef TASK_H
#define TASK_H

#include "misc.h"
#include "state.h"

#define MAX_PREDICATES 16
#define MAX_VARS       8
#define MAX_ATOMS      32
#define MAX_ACTIONS    64
#define MAX_OBJECTS    1024

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
	state* init;
	state* goal;
} task;

task task_init(void);
void task_free(task* def);
void task_print(const task* def);

#endif
