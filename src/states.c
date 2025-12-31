#include <stdint.h>
#include <stdlib.h>

#include "states.h"

struct states {
	size_t  len;
	size_t  cap;
	state** map;
};

states* states_new(void) {
	states* S = malloc(sizeof(states));

	S->len = 0;
	S->cap = 8;
	S->map = calloc(S->cap, sizeof(state*));

	return S;
}

size_t states_count(const states* s) {
	return s->len;
}

bool states_contains(const states* S, const state* s) {
	const uint64_t hash = state_hash(s);

	for (size_t o = 0; o < S->cap; o++) {
		const size_t i = (hash + o) % S->cap;
		if (!S->map[i])
			return false;
		if (state_equal(S->map[i], s))
			return true;
	}

	return false;
}

void states_insert(state** map, size_t cap, state* s) {
	const uint64_t hash = state_hash(s);

	for (size_t o = 0; o < cap; o++) {
		const uint64_t i = (hash + o) % cap;
		if (!map[i]) {
			map[i] = s;
			return;
		}
	}
}

void states_grow(states* S) {
	const size_t cap = 2 * S->cap;
	state**      map = calloc(cap, sizeof(state*));
	for (size_t i = 0; i < S->cap; i++)
		if (S->map[i])
			states_insert(map, cap, S->map[i]);
	free(S->map);
	S->cap = cap;
	S->map = map;
}

void states_add(states* S, state* s) {
	if (2 * S->len > S->cap)
		states_grow(S);
	states_insert(S->map, S->cap, s);
	S->len++;
}

void states_remove(states* S, state* s) {
	const uint64_t hash = state_hash(s);

	for (size_t o = 0; o < S->cap; o++) {
		const uint64_t i = (hash + o) % S->cap;
		if (S->map[i] && state_equal(S->map[i], s)) {
			S->map[i] = NULL;
			S->len--;
		}
	}
}

state* states_pop(states* S) {
	for (size_t i = 0; i < S->cap; i++)
		if (S->map[i]) {
			state* s = S->map[i];
			S->len--;
			S->map[i] = NULL;
			return s;
		}

	return NULL;
}
