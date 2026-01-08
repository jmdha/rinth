#include <assert.h>
#include <limits.h>
#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "state.h"

static uint64_t create_fact(size_t predicate, size_t len, const size_t* args) {
	uint64_t fact = (uint64_t)predicate << 48;
        for (size_t i = 0; i < len; i++)
                fact |= (uint64_t)args[i] << 16 * (uint64_t)i;
        return fact;
}

struct state {
        uint      count;
        uint64_t* facts;
};

state* state_new(void) {
        state* s = malloc(sizeof(state));
        s->count = 0;
        s->facts = NULL;
        return s;
}

state* state_clone(const state* s) {
        state* new = malloc(sizeof(state));
        new->count = s->count;
        new->facts = malloc(new->count * sizeof(uint64_t));
        memcpy(new->facts, s->facts, new->count * sizeof(uint64_t));
        return new;
}

static bool state_contains_bs(const uint64_t* facts, int left, int right, uint64_t fact) {
        while (left <= right) {
                const int mid = left + (right - left) / 2;
                if (facts[mid] == fact)
                        return true;
                if (facts[mid] < fact)
                        left = mid + 1;
                else
                        right = mid - 1;
        }
        return false;
}

static bool state_contains_(const struct state* s, uint64_t fact) {
        return state_contains_bs(s->facts, 0, s->count - 1, fact);
}

bool state_contains(const struct state* s, size_t predicate, size_t len, const size_t* args) {
        return state_contains_(s, create_fact(predicate, len, args));
}

bool state_equal(const struct state* a, const struct state* b) {
        if (a->count != b->count)
                return false;
	return memcmp(a->facts, b->facts, a->count * sizeof(uint64_t)) == 0;
}

bool state_covers(const struct state* a, const struct state* b) {
        for (size_t i = 0; i < b->count; i++)
                if (!state_contains_(a, b->facts[i]))
                        return false;
        return true;
}

size_t state_overlap(const struct state* a, const struct state* b) {
        size_t count = 0;
        for (size_t i = 0; i < a->count; i++)
                if (state_contains_(b, a->facts[i]))
                        count++;
        return count;
}

bool state_empty(const struct state* s) {
        return s->count == 0;
}

size_t state_count(const struct state* s) {
        return s->count;
}

size_t state_size(const struct state* s) {
        return sizeof(s->count) + s->count * sizeof(uint64_t);
}

uint64_t state_hash(const struct state* s) {
        uint64_t hash = 2166136261;
        for (size_t i = 0; i < s->count; i++) {
		hash ^= s->facts[i];
		hash *= 1099511628211;
	}
        return hash;
}

void state_free(struct state* s) {
        if (s->facts)
                free(s->facts);
        free(s);
}

void state_clear(struct state* s) {
        s->count = 0;
}

void state_insert(struct state* s, size_t predicate, size_t len, const size_t* args) {
        const uint64_t fact = create_fact(predicate, len, args);
        for (size_t i = 0; i < s->count; i++) {
                if (s->facts[i] == fact) {
                        return; // Already contains fact
                } else if (s->facts[i] > fact) {
                        s->facts = realloc(s->facts, sizeof(size_t) * ++s->count);
                        memmove(&s->facts[i + 1], &s->facts[i],
                                sizeof(size_t) * (s->count - i - 1));
                        s->facts[i] = fact;
                        return; // Inserted fact in sorted list
                }
        }
        s->facts               = realloc(s->facts, sizeof(uint64_t) * ++s->count);
        s->facts[s->count - 1] = fact;
}

void state_remove(struct state* s, size_t predicate, size_t len, const size_t* args) {
        const uint64_t fact = create_fact(predicate, len, args);
        for (uint i = 0; i < s->count; i++) {
                if (s->facts[i] == fact) {
                        memmove(&s->facts[i], &s->facts[i + 1],
                                sizeof(uint64_t) * (s->count - i - 1));
                        if (s->count == 1) {
                                free(s->facts);
                                s->facts = NULL;
                                s->count = 0;
                        } else {
                                s->facts = realloc(s->facts, sizeof(uint64_t) * --s->count);
                        }
                        return;
                } else if (s->facts[i] > fact) {
                        return;
                }
        }
}

struct state_iter {
        const state* state;
        uint         index;
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
        if (si->index >= si->state->count)
                return false;
        const uint64_t fact = si->state->facts[si->index++];
        *len                = 0;
        *pred               = ((uint16_t)fact) - 1;
        for (size_t i = 0; i < 3; i++) {
                const size_t arg = (uint16_t)(fact >> 16 * (i + 1));
                if (arg)
                        args[(*len)++] = arg - 1;
                else
                        break;
        }
        return true;
}
