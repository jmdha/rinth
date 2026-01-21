#include <stdio.h>

#include "eval.h"
#include "log.h"

size_t BEST_EVAL;
size_t (*F_EVAL)(const state*);

void eval_init(const task* def, eval_kind ekind) {
	BEST_EVAL = 0;
	switch (ekind) {
	case EVAL_GOAL_COUNT:
		F_EVAL = eval_goal_count;
		eval_init_goal_count(def, ekind);
		break;
	default:
		fprintf(stderr, "%d: unexpected eval kind\n", ekind);
		break;
	}
}

size_t eval(const state* s) {
	size_t val;

	val = F_EVAL(s);

	if (val > BEST_EVAL) {
		BEST_EVAL = val;
		INFO("New best heuristic val: %zu", val);
	}

	return val;
}
