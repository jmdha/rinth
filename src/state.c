#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <memory.h>

#include "state.h"

static u64 create_fact(uint len, u16* args) {
    u64 fact = 0;
    for (uint i = 0; i < len; i++)
        fact |= (u64) args[i] << 16 * i;
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

static bool state_contains_(const struct state* s, u64 fact) {
    for (uint i = 0; i < s->count; i++)
        if (fact == s->facts[i])
            return true;
    return false;
}

bool state_contains(const struct state* s, uint len, u16* args) {
    return state_contains_(s, create_fact(len, args));
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
        hash ^= s->facts[i] * s->facts[i];
    return hash;
}

void state_free(struct state* s) {
    if (s->facts)
        free(s->facts);
    free(s);
}

void state_insert(struct state* s, uint len, u16* args) {
    const u64 fact = create_fact(len, args);
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

void state_remove(struct state* s, uint len, u16* args) {
    const u64 fact = create_fact(len, args);
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
