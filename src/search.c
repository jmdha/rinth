#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stb_ds.h>

#include "log.h"
#include "search.h"
#include "eval.h"
#include "expand.h"
#include "state.h"
#include "statespace.h"

typedef struct {
	state* s;
	u64    val;
} nodeval;

bool solve(path* p, const state* init, const state* goal) {
    statespace* ss = statespace_new();
    nodeval* queue = NULL;

    nodeval start = { 
	.s   = state_clone(init), 
	.val = eval(init)
    };

    statespace_add(ss, start.s);
    arrpush(queue, start);

    while (arrlenu(queue) > 0) {
	size_t best_node = 0;
	u64    best_val  = 0;
	for (size_t i = 0; i < arrlenu(queue); i++) {
		const u64 val = queue[i].val;
		if (val > best_val) {
			best_node = i;
			best_val  = val;
		}
	}
	state* node = queue[best_node].s;
	arrdel(queue, best_node);

        uint   action;
        u16    args[MAX_VARIABLES];
        state* child;
        expand(node);
        while (expand_step(node, &action, args, &child)) {
	    if (statespace_contains(ss, child)) {
		free(child);
		continue;
	    }
            if (state_covers(child, goal))
                return true;
	    nodeval nval = {
		.s   = child,
		.val = eval(child)
	    };
	    statespace_add(ss, child);
            arrpush(queue, nval);
	    if (statespace_count(ss) % 1000 == 0)
		    INFO("SS Count: %zu", statespace_count(ss));
        }
    }
    return false;
}

bool reachable(const state* init, const state* goal) {
    return solve(NULL, init, goal);
}
