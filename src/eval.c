#include "eval.h"
#include "state.h"
#include <stdlib.h>

state* goal = NULL;

static void eval_fini() {
    if (goal)
        state_free(goal);
    goal = NULL;
}

void eval_init(const struct task* task) {
    atexit(eval_fini);
    eval_fini();
    goal = state_clone(task->goal);
}

uint goal_count(const state* s, const state* g) {
    return state_overlap(g, s);
}

uint eval(const state* s) {
    return goal_count(s, goal);
}
