#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <memory.h>

#include "state.h"

static u64 create_fact(u16 predicate, uint len, u16* args) {
    u64 fact = (u64) 1 + (u64) predicate;
    for (uint i = 0; i < len; i++)
        fact |= ((u64) 1 + (u64) args[i]) << 16 * (1 + i);
    return fact;
}

struct state {
    uint count;
    u64* facts;
};

state* state_new() {
    state* s = malloc(sizeof(state));
    s->count = 0;
    s->facts = NULL;
    return s;
}

state* state_clone(const state* old) {
    state* new = malloc(sizeof(state));
    new->count = old->count;
    new->facts = malloc(new->count * sizeof(u64));
    memcpy(new->facts, old->facts, new->count * sizeof(u64));
    return new;
}

static bool state_contains_bs(const u64* facts, int left, int right, u64 fact) {
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

static bool state_contains_(const struct state* s, u64 fact) {
    return state_contains_bs(s->facts, 0, s->count - 1, fact);
}

bool state_contains(const struct state* s, u16 predicate, uint len, u16* args) {
    return state_contains_(s, create_fact(predicate, len, args));
}

bool state_equal(const struct state* a, const struct state* b) {
    if (a->count != b->count) return false;
    for (uint i = 0; i < a->count; i++)
        if (a->facts[i] != b->facts[i])
            return false;
    return true;
}

bool state_covers(const struct state* a, const struct state* b) {
    for (uint i = 0; i < b->count; i++)
        if (!state_contains_(a, b->facts[i]))
            return false;
    return true;
}

uint state_overlap (const struct state* a, const struct state* b) {
    uint count = 0;
    for (uint i = 0; i < a->count; i++)
        if (state_contains_(b, a->facts[i]))
            count++;
    return count;
}

bool state_empty(const struct state* s) {
    return s->count == 0;
}

uint state_count(const struct state* s) {
    return s->count;
}

uint state_size(const struct state* s) {
    return sizeof(s->count) + s->count * sizeof(u64);
}

u64 state_hash(const struct state* s) {
    u64 hash = 0;
    for (uint i = 0; i < s->count; i++)
        hash = (hash << 5) + hash + s->facts[i];
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

void state_insert(struct state* s, u16 predicate, uint len, u16* args) {
    const u64 fact = create_fact(predicate, len, args);
    for (uint i = 0; i < s->count; i++) {
        if (s->facts[i] == fact) {
            return; // Already contains fact
        } else if (s->facts[i] > fact) {
            s->facts = realloc(s->facts, sizeof(u64) * ++s->count);
            memmove(&s->facts[i + 1], &s->facts[i], sizeof(u64) * (s->count - i - 1));
            s->facts[i] = fact;
            return; // Inserted fact in sorted list
        }
    }
    s->facts = realloc(s->facts, sizeof(u64) * ++s->count);
    s->facts[s->count - 1] = fact;
}

void state_remove(struct state* s, u16 predicate, uint len, u16* args) {
    const u64 fact = create_fact(predicate, len, args);
    for (uint i = 0; i < s->count; i++) {
        if (s->facts[i] == fact) {
            memmove(&s->facts[i], &s->facts[i + 1], sizeof(u64) * (s->count - i - 1));
            if (s->count == 1) {
                free(s->facts);
                s->facts = NULL;
                s->count = 0;
            } else {
                s->facts = realloc(s->facts, sizeof(u64) * --s->count);
            }
            return;
        } else if (s->facts[i] > fact) {
            return;
        }
    }
}

struct state_iter {
    const state* state;
    uint index;
};

state_iter* state_iter_new(const struct state* s) {
    state_iter* si = malloc(sizeof(state_iter));
    si->state = s;
    si->index = 0;
    return si;
}

void state_iter_free(struct state_iter* si) {
    free(si);
}

bool state_iter_step(struct state_iter* si, u16* pred, uint* len, u16* args) {
    if (si->index >= si->state->count) return false;
    const u64 fact = si->state->facts[si->index++];
    *len  = 0;
    *pred = ((u16) fact) - 1;
    for (uint i = 0; i < 3; i++) {
        const u16 arg = fact >> 16 * (i + 1);
        if (arg)
            args[(*len)++] = arg - 1;
        else
            break;
    }
    return true;
}
