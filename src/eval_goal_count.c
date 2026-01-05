#include "eval.h"

const state* goal = NULL;

void eval_init_goal_count(const task* def, eval_kind ekind) {
	goal = def->goal;
}

size_t eval_goal_count(const state* s) {
	return state_overlap(s, goal);
}
