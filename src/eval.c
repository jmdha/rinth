#include <stdio.h>

#include "eval.h"

size_t (*f_eval)(const state*);

void eval_init(const task* def, eval_kind ekind) {
	switch (ekind) {
	case EVAL_GOAL_COUNT:
		f_eval = eval_goal_count;
		eval_init_goal_count(def, ekind);
		break;
	default:
		fprintf(stderr, "%d: unexpected eval kind\n", ekind);
		break;
	}
}

size_t eval(const state* s) {
	return f_eval(s);
}
