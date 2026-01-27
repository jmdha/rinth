#include <stdlib.h>
#include <stdio.h>

#include "log.h"
#include "search.h"
#include "state_set.h"
#include "state_heap.h"
#include "expand.h"
#include "eval.h"

path solve_beam(const state* init, const state* goal) {
	path        p;
	size_t      action;
	size_t      args[64];
	state_set*  visited;
	state_heap* queue;
	state*      node;
	state*      child;

	visited = ss_new();
	queue   = sh_new();
	sh_push(queue, state_clone(init), 0);

	while ((node = sh_pop(queue)) != NULL) {
		ss_add(visited, state_clone(node));

		expand(node);
		while ((child = successor(&action, args))) {
			if (ss_contains(visited, child)) {
				state_free(child);
				continue;
			}

			if (state_covers(child, goal)) {
				INFO("SS: %zu %zu B", ss_count(visited), ss_size(visited));
				INFO("SH: %zu %zu B", sh_count(queue), sh_size(queue));
				state_free(child);
				state_free(node);
				ss_free(visited);
				sh_free(queue);
				p.len = 0;
				return p;
			}

			sh_push(queue, child, eval(child));
		}
		state_free(node);
		sh_reduce(queue, 1024);
	}
	p.len = SIZE_MAX;
	return p;
}
