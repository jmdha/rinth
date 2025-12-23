#include "eval.h"
#include "state.h"
#include <stdlib.h>

state* goal = NULL;

static void eval_fini(void) {
        if (goal)
                state_free(goal);
        goal = NULL;
}

void eval_init(const task_t* task) {
        atexit(eval_fini);
        eval_fini();
        goal = state_clone(task->goal);
}

uint eval(const state* s) {
        return state_overlap(s, goal);
}
