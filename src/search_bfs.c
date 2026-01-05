#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#include "search.h"
#include "expand.h"
#include "states.h"


path solve_bfs(const state* init, const state* goal) {
	path    p;
	size_t  action;
	size_t  args[64];
	state** queue;
	size_t  queue_len;
	size_t  queue_cap;

	queue_len = 1;
	queue_cap = 8;
	queue     = malloc(queue_cap * sizeof(state*));
	queue[0]  = state_clone(init);

	while (queue_len-- > 0) {
		state* node  = queue[0];
		state* child = NULL;

		// What even is time complexity
		memmove(&queue[0], &queue[1], queue_len * sizeof(state*));

		expand(node);
		while ((child = successor(&action, args))) {
			if (state_covers(child, goal)) {
				state_free(node);
				for (size_t i = 0; i < queue_len; i++)
					state_free(queue[i]);
				free(queue);
				p.len = 0;
				return p;
			}
			
			if (queue_len >= queue_cap) {
				queue_cap = 2 * queue_cap;
				queue     = realloc(queue, queue_cap * sizeof(state*));
			}

			queue[queue_len++] = child;
		}
	}

	free(queue);
	p.len = SIZE_MAX;
	return p;
}
