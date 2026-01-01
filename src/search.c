#include <stdlib.h>
#include <stdio.h>

#include "search.h"
#include "expand.h"
#include "states.h"

path solve(const state* init, const state* goal) {
	path p = { 0 };
	states* v = states_new();
	size_t qlen = 0;
	size_t qcap = 8;
	state** q = malloc(qcap * sizeof(state*));
	size_t gen = 0;

	q[qlen++] = state_clone(init);

	while (qlen > 0) {
		state* s = q[--qlen];
		states_add(v, s);

		expand(s);

		size_t action;
		size_t args[64];
		state* child;
		while ((child = successor(&action, args))) {
			gen++;
			if (states_contains(v, child)) {
				state_free(child);
				continue;
			}

			if (state_covers(child, goal)) {
				p.len = 0;
				printf("Visited: %zu\n", states_count(v));
				printf("Generated: %zu\n", gen);
				return p;
			}

			if (qlen >= qcap) {
				qcap *= 4;
				q = realloc(q, qcap * sizeof(state*));
			}

			q[qlen++] = child;
		}
	}

	return p;
}

bool reachable(const state* init, const state* goal) {
	return solve(init, goal).len != SIZE_MAX;
}
