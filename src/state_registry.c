#include <memory.h>
#include <stdlib.h>

#include "mem.h"
#include "state_registry.h"

typedef struct {
        uint64_t hash;
        uint64_t parent;
} entry;

static entry entry_new(const state* s, const state* p) {
        entry e = {0};

        e.hash = state_hash64(s);
        if (p)
                e.parent = state_hash64(p);

        return e;
}

static bool is_entry(const entry* e) {
        return e->hash;
}

static bool entry_equal(const entry* a, const entry* b) {
        return a->hash == b->hash;
}

static uint64_t entry_hash(const entry* e) {
        return e->hash;
}

struct state_registry {
        size_t len;
        size_t cap;
        entry* arr;
};

state_registry* sr_new(void) {
        state_registry* sr = malloc_(sizeof(state_registry));
        sr->len            = 0;
        sr->cap            = 8;
        sr->arr            = calloc_(8, sizeof(entry));
        return sr;
}

void sr_free(state_registry* sr) {
        if (sr->arr)
                free(sr->arr);
        free(sr);
}

size_t sr_count(const state_registry* sr) {
        size_t count = 0;

        for (size_t i = 0; i < sr->cap; i++)
                if (is_entry(&sr->arr[i]))
                        count++;

        return count;
}

size_t sr_size(const state_registry* sr) {
        return sizeof(state_registry) + sr->cap * sizeof(entry);
}

bool sr_contains(const state_registry* sr, const state* s) {
        entry    e    = entry_new(s, NULL);
        uint64_t hash = entry_hash(&e);
        for (size_t o = 0; o < sr->cap; o++) {
                const size_t i = (hash + o) % sr->cap;
                if (!is_entry(&sr->arr[i]))
                        return false;
                if (entry_equal(&e, &sr->arr[i]))
                        return true;
        }
        return false;
}

void sr_insert(entry* entries, size_t cap, entry e) {
        uint64_t hash = entry_hash(&e);
        for (size_t o = 0; o < cap; o++) {
                const size_t i = (hash + o) % cap;
                if (!is_entry(&entries[i])) {
                        entries[i] = e;
                        break;
                }
        }
}

void sr_grow(state_registry* sr) {
        const size_t cap = 4 * sr->cap;
        entry*       arr = calloc_(cap, sizeof(entry));
        for (size_t i = 0; i < sr->cap; i++)
                if (!is_entry(&arr[i]))
                        sr_insert(arr, cap, sr->arr[i]);
        free(sr->arr);
        sr->cap = cap;
        sr->arr = arr;
}

void sr_push(state_registry* sr, const state* s, const state* p) {
        if (4 * sr->len > sr->cap)
                sr_grow(sr);
        sr_insert(sr->arr, sr->cap, entry_new(s, p));
        sr->len++;
}

bool sr_ischild(state_registry* sr, const state* p, const state* c) {}
