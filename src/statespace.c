#include <stdlib.h>

#include "statespace.h"

struct statespace {
	state** map; // Maps key to elements index
	size_t  count;
	size_t  capacity;
};

statespace* statespace_new() {
	statespace* ss = malloc(sizeof(struct statespace));

	ss->count    = 0;
	ss->capacity = 8;
	ss->map      = calloc(ss->capacity, sizeof(state*));

	return ss;
}

size_t statespace_count(const statespace* ss) {
	return ss->count;
}

bool statespace_contains(const statespace* ss, const state* s) {
	const u64 hash = state_hash(s);
	for (size_t o = 0; o < ss->capacity; o++) {
		const u64 i = (hash + o) % ss->capacity;
		if (ss->map[i] == NULL)
			return false;
		if (state_equal(ss->map[i], s))
			return true;
	}
	return false;
}

static void statespace_insert(state** m, size_t capacity, state* s) {
	const u64 hash = state_hash(s);
	for (size_t o = 0; o < capacity; o++) {
		const u64 i = (hash + o) % capacity;
		if (m[i] == NULL) {
			m[i] = s;
			return;
		}
	}
	exit(1);
}

static void statespace_grow(statespace* ss) {
	const size_t new_cap = 2 * ss->capacity;
	state** new_map      = calloc(new_cap, sizeof(state*));
	for (size_t i = 0; i < ss->capacity; i++)
		if (ss->map[i] != NULL)
			statespace_insert(new_map, new_cap, ss->map[i]);
	free(ss->map);
	ss->map      = new_map;
	ss->capacity = new_cap;
}

void statespace_add(statespace* ss, state* s) {
	if (ss->count * 2 > ss->capacity)
		statespace_grow(ss);
	statespace_insert(ss->map, ss->capacity, s);
	ss->count++;
}
