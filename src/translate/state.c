#include <stdio.h>
#include <stdlib.h>

#include "log.h"
#include "state.h"

// TODO: Implement tree type state
// I'm thinking where leaf nodes are buckets with start and end range of objects

// HACK: This is slow
struct state {
    uint count;
    struct fact *facts;
};

struct state *state() {
    struct state *s = (struct state *)malloc(sizeof(struct state));
    s->count        = 0;
    s->facts        = NULL;
    return s;
}

void state_free(struct state *s) {
    free(s->facts);
    free(s);
}

bool state_contains(const struct state *s, struct fact *f) {
    for (uint i = 0; i < s->count; i++)
        if (fact_equal(f, &s->facts[i])) return true;
    return false;
}

bool state_equal(const struct state *a, const struct state *b) {
    for (uint i = 0; i < a->count; i++)
        if (!state_contains(b, &a->facts[i])) return false;
    for (uint i = 0; i < b->count; i++)
        if (!state_contains(a, &b->facts[i])) return false;
    return true;
}

bool state_empty(const struct state *s) { return s->count == 0; }

uint state_count(const struct state *s) { return s->count; }

uint state_size(const struct state *s) { return sizeof(uint) + s->count * sizeof(fact); }

void state_insert(struct state *s, struct fact *f) {
    if (state_contains(s, f)) return;
    const uint count = s->count + 1;
    s->facts         = realloc(s->facts, count * sizeof(struct fact));
    if (!s->facts) {
        perror("Failed to allocate memory for state insertion");
        exit(1);
    }
    s->facts[s->count] = *f;
    s->count           = count;
}

void state_remove(struct state *s, struct fact *f) {
    for (uint i = 0; i < s->count; i++) {
        if (fact_equal(&s->facts[i], f)) {
            s->facts[i] = s->facts[s->count - 1];
            s->count    = s->count - 1;
            if (!s->count) {
                free(s->facts);
                s->facts = NULL;
            } else {
                s->facts = realloc(s->facts, s->count * sizeof(struct fact));
                if (!s->facts) {
                    perror("Failed to allocate memory on fact removal");
                    exit(1);
                }
            }
            break;
        }
    }
}

void state_iterate(const struct state *s, void (*f)(struct fact *)) {
    for (uint i = 0; i < s->count; i++)
        f(&s->facts[i]);
}
