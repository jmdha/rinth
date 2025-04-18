#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stb_ds.h>

#include "search.h"
#include "eval.h"
#include "expand.h"
#include "state.h"

bool contains(state** states, state* state) {
    for (size_t i = 0; i < arrlenu(states); i++)
        if (state_equal(states[i], state))
            return true;
    return false;
}

bool solve(path* p, const state* init, const state* goal) {
    size_t  mark   = 0;
    state** states = NULL;
    arrpush(states, state_clone(init));

    while (mark < arrlenu(states)) {
        size_t best_node = 0;
        u64    best_val  = 0;
        for (size_t i = mark; i < arrlenu(states); i++) {
            u64 val = eval(states[i]);
            if (val >= best_val) {
                best_node = i;
                best_val  = val;
            }
        }
        state* node       = states[best_node];
        states[best_node] = states[mark];
        states[mark]      = node;
        mark++;
        uint   action;
        u16    args[MAX_VARIABLES];
        state* child;
        expand(node);
        while (expand_step(node, &action, args, &child)) {
            if (contains(states, child)) {
                free(child);
                continue;
            }
            if (state_covers(child, goal))
                return true;
            arrpush(states, child);
        }
    }
    return false;
}

bool reachable(const state* init, const state* goal) {
    return solve(NULL, init, goal);
}
