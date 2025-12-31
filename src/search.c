#include <stdlib.h>
#include <stdio.h>

#include "search.h"
#include "expand.h"
#include "states.h"

path solve(const state* init, const state* goal) {
	path p = { 0 };
	states* q = states_new();
	states* v = states_new();

	states_add(q, state_clone(init));

	while (states_count > 0) {
		state* s = states_pop(q);
		states_add(v, s);

		expand(s);

		size_t action;
		size_t args[64];
		state* child;
		while ((child = successor(&action, args))) {
			if (states_contains(q, child) || states_contains(v, child)) {
				state_free(child);
				continue;
			}

			if (state_covers(child, goal)) {
				p.len = 0;
				printf("Visited: %zu | Queue: %zu\n", states_count(v), states_count(q));
				return p;
			}

			states_add(q, child);
		}
	}

	return p;
}

bool reachable(const state* init, const state* goal) {
	return solve(init, goal).len != SIZE_MAX;
}
