#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "expand.h"
#include "log.h"

static const task*   TASK = NULL;
static const state*  STATE = NULL;
static const action* ACTIONS = NULL;

void (*f_expand)(const state*);
bool (*f_expand_step)(size_t*, size_t*);

static size_t SUCCESSORS = 0;

static void fini(void) {
	INFO("Successors generated %zu", SUCCESSORS);
}

void expand_init(const task* t, expand_kind ekind) {
	atexit(fini);
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
	for (size_t i = 0; a->eff_neg[i].predicate != SIZE_MAX; i++) {
                const atom* atom = &a->eff_neg[i];
                for (size_t i = 0; i < atom->arity; i++)
                        buf[i] = args[atom->vars[i]];
		state_remove(s, atom->predicate, atom->arity, buf);
	}

	for (size_t i = 0; a->eff_pos[i].predicate != SIZE_MAX; i++) {
                const atom* atom = &a->eff_pos[i];
                for (size_t i = 0; i < atom->arity; i++)
                        buf[i] = args[atom->vars[i]];
		state_insert(s, atom->predicate, atom->arity, buf);
	}

	SUCCESSORS++;
	return s;
}
