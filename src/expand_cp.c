#include <stdint.h>

#include "algo.h"
#include "expand.h"
#include "misc.h"

size_t        COUNT   = SIZE_MAX;
const action* ACTIONS = NULL;
size_t        ACTION  = SIZE_MAX;
size_t***     ARGS    = NULL;
const state*  STATE   = NULL;
cp_iter*      CP      = NULL;

void expand_init_cp(const task* def) {
	COUNT = 0;
        ACTIONS = def->actions;
        for (size_t i = 0; ACTIONS[i].name.ptr; i++)
                COUNT++;
        // TODO: Make args influenced my statics
        ARGS = malloc(COUNT * sizeof(size_t**));
        for (size_t i = 0; ACTIONS[i].name.ptr; i++)
                ARGS[i] = malloc(ACTIONS[i].arity * sizeof(size_t*));
        for (size_t i = 0; ACTIONS[i].name.ptr; i++)
                for (size_t t = 0; t < ACTIONS[i].arity; t++) {
                        ARGS[i][t] = malloc((1 + slen(def->objects)) * sizeof(size_t));
                        for (size_t j = 0; j < slen(def->objects); j++)
                                ARGS[i][t][j] = j;
			ARGS[i][t][slen(def->objects)] = SIZE_MAX;
                }
}

void expand_cp(const state* s) {
        if (CP) {
                cp_free(CP);
                CP = NULL;
        }
        ACTION = 0;
        STATE  = s;
}

bool is_legal(const state* s, const action* a, const size_t* args) {
        size_t buf[32];
        for (size_t i = 0; a->pre_pos[i].predicate != SIZE_MAX; i++) {
                const atom* atom = &a->pre_pos[i];
                for (size_t i = 0; i < atom->arity; i++)
                        buf[i] = args[atom->vars[i]];
                if (!state_contains(s, atom->predicate, atom->arity, buf))
                        return false;
        }
        for (size_t i = 0; a->pre_neg[i].predicate != SIZE_MAX; i++) {
                const atom* atom = &a->pre_neg[i];
                for (size_t i = 0; i < atom->arity; i++)
                        buf[i] = args[atom->vars[i]];
                if (state_contains(s, atom->predicate, atom->arity, buf))
                        return false;
        }
        return true;
}

bool expand_step_cp(size_t* action, size_t* args) {
	do {
		if (ACTION >= COUNT)
			return false;
		if (!CP)
			CP = cp_init(ARGS[ACTION]);
        	if (!cp_step(CP, args)) {
        	        ACTION++;
        	        if (CP) {
        	                cp_free(CP);
				CP = NULL;
			}
			continue;
        	}
	} while (!is_legal(STATE, &ACTIONS[ACTION], args));

	*action = ACTION;
        return ACTION < COUNT;
}
