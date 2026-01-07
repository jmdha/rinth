#include "search.h"
#include "expand.h"
#include "state_queue.h"

path solve_bfs(const state* init, const state* goal) {
	path         p;
	size_t       action;
	size_t       args[64];
	state*       node;
	state*       child;
	state_queue* sq;

	sq = sq_new();
	sq_push(sq, state_clone(init));

	while ((node = sq_pop(sq))) {
		expand(node);
		while ((child = successor(&action, args))) {
			if (state_covers(child, goal)) {
				state_free(node);
				state_free(child);
				sq_free(sq, true);
				p.len = 0;
				return p;
			}
			sq_push(sq, child);
		}
		state_free(node);
	}
	sq_free(sq, true);
	p.len = SIZE_MAX;
	return p;
}
