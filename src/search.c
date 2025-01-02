#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "eval.h"
#include "search.h"
#include "bounds.h"
#include "expand.h"
#include "log.h"
#include "state.h"

bool solve(path* p, const state* init, const state* goal) {
    state* node = state_clone(init);

    u64 expansions = 0;
    u64 states     = 0;
    u64 sen        = 0;
    while (true) {
        uint   action;
        u16    args[MAX_VARIABLES];
        state* best_child = NULL;
        uint   best_val = 0;
        state* child;
        expand(node);
        expansions++;
        while (expand_step(node, &action, args, &child)) {
            states++;
            if (state_covers(child, goal))
                return true;
            const uint val = eval(child);
            if (val >= best_val && (!best_child || rand() % 2 == 0)) {
                if (best_child)
                    state_free(best_child);
                best_child = child;
                best_val   = val;
                if (best_val > sen) {
                    INFO("New best val of %d (%d - %d)", best_val, expansions, states);
                    sen = best_val;
                }
            } else {
                state_free(child);
            }
        }
        state_free(node);
        if (!best_child)
            return false;
        node = best_child;
    }
}

bool reachable(const state* init, const state* goal) {
    return solve(NULL, init, goal);
}
