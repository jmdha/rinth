#include <stdint.h>

#include "task.h"

task task_init(void) {
	task def = { 0 };

	for (size_t i = 0; i < MAX_ACTIONS; i++) {
		for (size_t t = 0; t < MAX_ATOMS; t++) {
			def.actions[i].pre_pos[t].predicate = SIZE_MAX;
			def.actions[i].pre_pos[t].arity = SIZE_MAX;
			for (size_t j = 0; j < MAX_VARS; j++)
				def.actions[i].pre_pos[t].vars[j] = SIZE_MAX;
		}
		for (size_t t = 0; t < MAX_ATOMS; t++) {
			def.actions[i].pre_neg[t].predicate = SIZE_MAX;
			def.actions[i].pre_neg[t].arity = SIZE_MAX;
			for (size_t j = 0; j < MAX_VARS; j++)
				def.actions[i].pre_neg[t].vars[j] = SIZE_MAX;
		}
		for (size_t t = 0; t < MAX_ATOMS; t++) {
			def.actions[i].eff_pos[t].predicate = SIZE_MAX;
			def.actions[i].eff_pos[t].arity = SIZE_MAX;
			for (size_t j = 0; j < MAX_VARS; j++)
				def.actions[i].eff_pos[t].vars[j] = SIZE_MAX;
		}
		for (size_t t = 0; t < MAX_ATOMS; t++) {
			def.actions[i].eff_neg[t].predicate = SIZE_MAX;
			def.actions[i].eff_neg[t].arity = SIZE_MAX;
			for (size_t j = 0; j < MAX_VARS; j++)
				def.actions[i].eff_neg[t].vars[j] = SIZE_MAX;
		}
	}

	for (size_t i = 0; i < MAX_INITS; i++)
		def.inits[i].predicate = SIZE_MAX;

	for (size_t i = 0; i < MAX_GOALS; i++)
		def.goals[i].predicate = SIZE_MAX;

	return def;
}
