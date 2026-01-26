#include <stdint.h>
#include <stdlib.h>
#include <errno.h>

#include "state_set.h"
#include "log.h"

#define GROWTH_FACTOR 4

struct state_set {
	size_t  len;
	size_t  cap;
	state** set;
};

state_set* ss_new(void) {
	state_set* ss = malloc(sizeof(state_set));

	ss->len = 0;
	ss->cap = 8;
	ss->set = calloc(ss->cap, sizeof(state*));
	if (!ss->set)
		exit(errno);

	return ss;
}

void ss_free(state_set* ss) {
	free(ss->set);
	free(ss);
}

size_t ss_count(const state_set* ss) {
	return ss->len;
}

bool ss_contains(const state_set* ss, const state* s) {
	const uint64_t hash = state_hash(s);

	for (size_t o = 0; o < ss->cap; o++) {
		const size_t i = (hash + o) % ss->cap;
		if (!ss->set[i])
			return false;
		if (state_equal(ss->set[i], s))
			return true;
	}

	return false;
}

void ss_insert(state** set, size_t cap, state* s) {
	const uint64_t hash = state_hash(s);

	for (size_t o = 0; o < cap; o++) {
		const uint64_t i = (hash + o) % cap;
		if (!set[i]) {
			set[i] = s;
			break;
		}
	}
}

void ss_grow(state_set* ss) {
	const size_t cap = GROWTH_FACTOR * ss->cap;
	state**      set = calloc(cap, sizeof(state*));
	if (!set)
		exit(errno);
	for (size_t i = 0; i < ss->cap; i++)
		if (ss->set[i])
			ss_insert(set, cap, ss->set[i]);
	free(ss->set);
	ss->cap = cap;
	ss->set = set;
}

void ss_add(state_set* ss, state* s) {
	if (GROWTH_FACTOR * ss->len > ss->cap) {
		ss_grow(ss);
		size_t size = sizeof(state_set);
		for (size_t i = 0; i < ss->cap; i++)
			if (ss->set[i])
				size += state_size(ss->set[i]);
		INFO("State Set: %zu %zu %zu MB", ss->len, ss->cap, size / 1000 / 1000);
	}
	ss_insert(ss->set, ss->cap, s);
	ss->len++;
}

void ss_remove(state_set* ss, state* s) {
	const uint64_t hash = state_hash(s);

	for (size_t o = 0; o < ss->cap; o++) {
		const uint64_t i = (hash + o) % ss->cap;
		if (ss->set[i] && state_equal(ss->set[i], s)) {
			ss->set[i] = NULL;
			ss->len--;
		}
	}
}
