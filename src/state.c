#include <assert.h>
#include <limits.h>
#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "state.h"

// State is implemented as a perfect hash set
struct state {
	size_t    len; // Number of elements in state
	size_t    cap; // Capcity of buf
	uint64_t* buf; // Fact buffer
};

static uint64_t fact_new(size_t predicate, size_t len, const size_t* args) {
	uint64_t fact = (uint64_t)predicate << 48;
        for (size_t i = 0; i < len; i++)
                fact |= (uint64_t)args[i] << 16 * (uint64_t)i;
        return fact;
}

static size_t fact_index(size_t cap, uint64_t fact) {
	return fact % cap;
}

state* state_new(void) {
        state* s = malloc(sizeof(state));
	s->len   = 0;
	s->cap   = 8;
	s->buf   = malloc(s->cap * sizeof(uint64_t));
	for (size_t i = 0; i < s->cap; i++)
		s->buf[i] = SIZE_MAX;
        return s;
}

state* state_clone(const state* s) {
	state* n = malloc(sizeof(state));
	n->len   = s->len;
	n->cap   = s->cap;
	n->buf   = malloc(n->cap * sizeof(uint64_t));
	memcpy(n->buf, s->buf, s->cap * sizeof(uint64_t));
	return n;
}

bool state_contains_(const struct state* s, uint64_t fact) {
	return fact == s->buf[fact_index(s->cap, fact)];
}

bool state_contains(const struct state* s, size_t predicate, size_t len, const size_t* args) {
	return state_contains_(s, fact_new(predicate, len, args));
}

bool state_equal(const struct state* a, const struct state* b) {
	if (state_count(a) != state_count(b))
		return false;
	return state_overlap(a, b) == state_count(a);
}

bool state_covers(const struct state* a, const struct state* b) {
	if (state_count(a) < state_count(b))
		return false;
	return state_overlap(a, b) == state_count(b);
}

size_t state_overlap(const struct state* a, const struct state* b) {
	size_t count = 0;
	for (size_t i = 0; i < a->cap; i++) {
		if (a->buf[i] == SIZE_MAX)
			continue;
		if (state_contains_(b, a->buf[i]))
			count++;
	}
	return count;
}

bool state_empty(const struct state* s) {
	return state_count(s) == 0;
}

size_t state_count(const struct state* s) {
	return s->len;
}

size_t state_size(const struct state* s) {
	return sizeof(state) + s->cap * sizeof(uint64_t);
}

uint64_t state_hash(const struct state* s) {
	uint64_t hash = 2166136261;
	for (size_t i = 0; i < s->cap; i++) {
		if (s->buf[i] == SIZE_MAX)
			continue;
		hash ^= s->buf[i];
		hash *= 1099511628211;
	}
	return hash;
}

void state_free(struct state* s) {
	free(s->buf);
	free(s);
}

void state_clear(struct state* s) {
	free(s->buf);
	s->len = 0;
	s->cap = 8;
	s->buf = malloc(s->cap * sizeof(uint64_t));
	for (size_t i = 0; i < s->cap; i++)
		s->buf[i] = SIZE_MAX;
}

static bool insert_fact(uint64_t* buf, size_t cap, uint64_t fact) {
	uint64_t index = fact_index(cap, fact);
	if (buf[index] != SIZE_MAX)
		return false;
	buf[index] = fact;
	return true;
}

static bool copy_buf(uint64_t* buf, size_t cap, size_t old_cap, const uint64_t* old_buf) {
	for (size_t i = 0; i < old_cap; i++) {
		uint64_t fact = old_buf[i];
		if (fact == SIZE_MAX)
			continue;
		if (!insert_fact(buf, cap, fact))
			return false;
	}
	return true;
}

static bool update_buf(uint64_t* buf, size_t cap, uint64_t fact, size_t old_cap, uint64_t* old_buf) {
	if (!copy_buf(buf, cap, old_cap, old_buf))
		return false;
	if (!insert_fact(buf, cap, fact))
		return false;
	return true;
}

void state_insert(struct state* s, size_t predicate, size_t len, const size_t* args) {
	uint64_t fact  = fact_new(predicate, len, args);
	size_t   index = fact_index(s->cap, fact);
	if (s->buf[index] == fact)
		return; // Already in state
	s->len++;
	if (s->buf[index] == SIZE_MAX) {
		s->buf[index] = fact;
		return; // Inserted without rebuild required
	}
	size_t    cap = 2 * s->len;
	uint64_t* buf = NULL;
	do {
		cap++;
		buf = realloc(buf, cap * sizeof(uint64_t));
		for (size_t i = 0; i < cap; i++)
			buf[i] = SIZE_MAX;
	} while (!update_buf(buf, cap, fact, s->cap, s->buf));
	free(s->buf);
	s->cap = cap;
	s->buf = buf;
}

void state_remove(struct state* s, size_t predicate, size_t len, const size_t* args) {
	uint64_t fact  = fact_new(predicate, len, args);
	size_t   index = fact_index(s->cap, fact);
	if (s->buf[index] != fact)
		return; // Not in state
	s->buf[index] = SIZE_MAX;
	s->len--;
}

struct state_iter {
        const state* state;
        size_t       index;
};

state_iter* state_iter_new(const struct state* s) {
        state_iter* si = malloc(sizeof(state_iter));
	si->state      = s;
	si->index      = 0;
        return si;
}

void state_iter_free(struct state_iter* si) {
        free(si);
}

bool state_iter_step(struct state_iter* si, size_t* pred, size_t* len, size_t* args) {
	while (si->index < si->state->cap) {
		uint64_t fact = si->state->buf[si->index++];
		if (fact == SIZE_MAX)
			continue;

		*len  = 0;
        	*pred = (uint16_t)fact;
        	for (size_t i = 0; i < 3; i++) {
        	        const size_t arg = (uint16_t)(fact >> 16 * (i + 1));
        	        if (arg)
        	                args[(*len)++] = arg;
        	        else
        	                break;
        	}
		return true;
	}
	return false;
}
