#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#include "search.h"
#include "expand.h"
#include "states.h"

path solve_bfs(const state* init, const state* goal) {
	path p = { 0 };
	states* v = states_new();
	size_t qlen = 0;
	size_t qcap = 8;
	state** q = malloc(qcap * sizeof(state*));

	q[qlen++] = state_clone(init);

	while (qlen > 0) {
		state* s = q[0];
		memmove(&q[0], &q[1], --qlen);
		states_add(v, s);

		expand(s);

		size_t action;
		size_t args[64];
		state* child;
		while ((child = successor(&action, args))) {
			if (states_contains(v, child)) {
				state_free(child);
				continue;
			}

			if (state_covers(child, goal)) {
				p.len = 0;
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
