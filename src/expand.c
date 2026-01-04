#include <stdint.h>
#include <stdio.h>

#include "expand.h"

static const task*   TASK = NULL;
static const state*  STATE = NULL;
static const action* ACTIONS = NULL;

void (*f_expand)(const state*);
bool (*f_expand_step)(size_t*, size_t*);

void expand_init(const task* t, expand_kind ekind) {
	if (ekind == EXPAND_NONE)
		ekind = EXPAND_CP;

	switch (ekind) {
	case EXPAND_CP:
		expand_init_cp(t);
		f_expand = expand_cp;
		f_expand_step = expand_step_cp;
		break;
	case EXPAND_SQLITE:
		expand_init_sqlite(t);
		f_expand = expand_sqlite;
		f_expand_step = expand_step_sqlite;
		break;
	default:
		fprintf(stderr, "%d: unexpected expansion kind\n", ekind);
		break;
	}

	TASK = t;
	ACTIONS = t->actions;
}

void expand(const state* s) {
        f_expand(s);

	STATE = s;
}

bool expand_step(size_t* action, size_t* args) {
        return f_expand_step(action, args);
}

state* successor(size_t* action_index, size_t* args) {
	if (!expand_step(action_index, args))
		return NULL;

	state* s = state_clone(STATE);
	const action* a = &ACTIONS[*action_index];

        size_t buf[32];
	for (size_t i = 0; a->eff_pos[i].predicate != SIZE_MAX; i++) {
                const atom* atom = &a->eff_pos[i];
                for (size_t i = 0; i < atom->arity; i++)
                        buf[i] = args[atom->vars[i]];
		state_insert(s, atom->predicate, atom->arity, buf);
	}

	for (size_t i = 0; a->eff_neg[i].predicate != SIZE_MAX; i++) {
                const atom* atom = &a->eff_neg[i];
                for (size_t i = 0; i < atom->arity; i++)
                        buf[i] = args[atom->vars[i]];
		state_remove(s, atom->predicate, atom->arity, buf);
	}

	//state_iter* si = state_iter_new(s);
	//size_t si_pred;
	//size_t si_len;
	//size_t si_args[256];
	//printf("----STATE----\n");
	//while (state_iter_step(si, &si_pred, &si_len, si_args)) {
	//	printf("(");
	//	printf("%.*s", (int) TASK->predicates[si_pred].len, TASK->predicates[si_pred].ptr);
	//	for (size_t i = 0; i < si_len; i++)
	//		printf(" %.*s", (int) TASK->objects[si_args[i]].len, TASK->objects[si_args[i]].ptr);
	//	printf(")\n");
	//}
	//state_iter_free(si);

	return s;
}
