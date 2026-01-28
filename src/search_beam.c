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
	state_heap* iqueue;
	state*      node;
	state*      child;

	visited = ss_new();
	queue   = sh_new();
	sh_push(queue, state_clone(init), 0);

	while (!sh_empty(queue)) {
		iqueue = sh_new();
		while ((node = sh_pop(queue)) != NULL) {
			ss_add(visited, state_clone(node));
			expand(node);
			while ((child = successor(&action, args))) {
				if (ss_contains(visited, child)) {
					state_free(child);
					continue;
				}
				if (state_covers(child, goal)) {
					state_free(child);
					state_free(node);
					sh_free(queue);
					sh_free(iqueue);
					p.len = 0;
					return p;
				}
				sh_push(iqueue, child, eval(child));
			}
			state_free(node);
		}
		sh_free(queue);
		queue = iqueue;
		sh_reduce(queue, 8);
	}

	p.len = SIZE_MAX;
	return p;
}
