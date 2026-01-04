#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#include "search.h"
#include "expand.h"
#include "states.h"

typedef struct node {
	state* s;
	struct node* n;
} node;

path solve_bfs(const state* init, const state* goal) {
	path   p;
	size_t action;
	size_t args[64];
	node*  queue;

	queue    = malloc(sizeof(node));
	queue->s = state_clone(init);
	queue->n = NULL;

	while (queue) {
		node*  curr;
		state* child;

		curr  = queue;
		queue = queue->n;

		expand(curr->s);
		while ((child = successor(&action, args))) {
			if (state_covers(child, goal)) {
				p.len = 0;
				goto DONE;
			}
			
			node* child_node = malloc(sizeof(node));
			child_node->s    = child;
			child_node->n    = queue;
			queue            = child_node;
		}
		state_free(curr->s);
		free(curr);
	}

	p.len = SIZE_MAX;

DONE:
	while (queue) {
		node* next = queue->n;
		state_free(queue->s);
		free(queue);
		queue = next;
	}
	return p;
}
