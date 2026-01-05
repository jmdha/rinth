#include <stdlib.h>

#include "search.h"
#include "states.h"
#include "expand.h"
#include "eval.h"

typedef struct node_t {
	state* s;
	size_t v;
} node_t;

path solve_gbfs(const state* init, const state* goal) {
	path    p;
	size_t  action;
	size_t  args[64];
	states* visited;
	node_t* queue;
	size_t  queue_len;
	size_t  queue_cap;

	visited    = states_new();
	queue_len  = 1;
	queue_cap  = 8;
	queue      = malloc(queue_cap * sizeof(node_t));
	queue[0].s = state_clone(init);
	queue[0].v = 0;

	while (queue_len > 0) {
		state* node  = NULL;
		state* child = NULL;

		size_t best     = 0;
		size_t best_val = 0;

		for (size_t i = 0; i < queue_len; i++)
			if (queue[i].v > best_val) {
				best     = i;
				best_val = queue[i].v;
			}

		node        = queue[best].s;
		queue[best] = queue[--queue_len];

		expand(node);
		while ((child = successor(&action, args))) {
			if (states_contains(visited, child)) {
				state_free(child);
				continue;
			}

			if (state_covers(child, goal)) {
				for (size_t i = 0; i < queue_len; i++)
					state_free(queue[i].s);
				free(queue);
				state_free(child);
				state_free(node);
				states_free(visited);
				p.len = 0;
				return p;
			}

			if (queue_len >= queue_cap) {
				queue_cap = 2 * queue_cap;
				queue     = realloc(queue, queue_cap * sizeof(node_t));
			}

			queue[queue_len].s = child;
			queue[queue_len].v = eval(child);
			queue_len++;
		}
		state_free(node);
	}
	p.len = SIZE_MAX;
	return p;
}
