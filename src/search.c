#include <stdlib.h>
#include <stdio.h>

#include "search.h"
#include "expand.h"

path solve(const state* init, const state* goal) {
	path p = { 0 };
	state** q = malloc(4 * sizeof(state*));
	state** v = malloc(4 * sizeof(state*));
	size_t q_len = 0;
	size_t v_len = 0;
	size_t q_cap = 4;
	size_t v_cap = 4;
	p.len = SIZE_MAX;

	q[q_len] = state_clone(init);
	q_len = 1;

	while (q_len > 0) {
		state* s = q[--q_len];

		if (v_len + 1 >= v_cap) {
			v_cap = 2 * v_cap;
			v = realloc(v, v_cap * sizeof(state*));
		}
		v[v_len++] = s;

		expand(s);

		size_t action;
		size_t args[64];
		state* child;
		while ((child = successor(&action, args))) {
			bool duplicate = false;
			for (size_t i = 0; i < q_len; i++)
				if (state_equal(q[i], child)) {
					duplicate = true;
					break;
				}

			for (size_t i = 0; i < v_len; i++)
				if (state_equal(v[i], child)) {
					duplicate = true;
					break;
				}

			if (duplicate) {
				state_free(child);
				continue;
			}

			if (state_covers(child, goal)) {
				p.len = 0;
				return p;
			}

			if (q_len + 1 >= q_cap) {
				q_cap = 2 * q_cap;
				q = realloc(q, q_cap * sizeof(state*));
			}

			q[q_len++] = child;
		}
	}

	return p;
}

bool reachable(const state* init, const state* goal) {
	return solve(init, goal).len != SIZE_MAX;
}
