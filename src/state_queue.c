#include <stdlib.h>
#include <memory.h>

#include "state_queue.h"

struct state_queue {
	size_t  len;
	size_t  cap;
	state** ele;
};

state_queue* sq_new(void) {
	state_queue* sq = malloc(sizeof(state_queue));

	sq->len = 0;
	sq->cap = 8;
	sq->ele = malloc(sq->cap * sizeof(state*));

	return sq;
}

void sq_free(state_queue* sq, bool free_states) {
	if (free_states)
		for (size_t i = 0; i < sq->len; i++)
			state_free(sq->ele[i]);

	free(sq->ele);
	free(sq);
}

const state* sq_front(const state_queue* sq) {
	if (sq_empty(sq))
		return NULL;

	return sq->ele[0];
}

const state* sq_back(const state_queue* sq) {
	if (sq_empty(sq))
		return NULL;

	return sq->ele[sq->len - 1];
}

bool sq_empty(const state_queue* sq) {
	return sq_size(sq) == 0;
}

size_t sq_size(const state_queue* sq) {
	return sq->len;
}

void sq_push(state_queue* sq, state* s) {
	if (sq->len >= sq->cap) {
		sq->cap = 2 * sq->cap;
		sq->ele = realloc(sq->ele, sq->cap * sizeof(state*));
	}

	sq->ele[sq->len++] = s;
}

state* sq_pop(state_queue* sq) {
	if (sq_empty(sq))
		return NULL;

	state* s = sq->ele[0];
	memmove(&sq->ele[0], &sq->ele[1], --sq->len * sizeof(state*));

	return s;
}
