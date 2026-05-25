#include <memory.h>
#include <stdio.h>
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
        else
                e.parent = 0;

        return e;
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

size_t sr_get(const state_registry* sr, uint64_t hash) {
        for (size_t o = 0; o < sr->cap; o++) {
                const size_t i = (hash + o) % sr->cap;
                if (!sr->arr[i].hash)
                        return SIZE_MAX;
                if (hash == sr->arr[i].hash)
                        return i;
        }
        return SIZE_MAX;
}

size_t sr_count(const state_registry* sr) {
        size_t count = 0;

        for (size_t i = 0; i < sr->cap; i++)
                if (sr->arr[i].hash)
                        count++;

        return count;
}

size_t sr_size(const state_registry* sr) {
        return sizeof(state_registry) + sr->cap * sizeof(entry);
}

bool sr_contains(const state_registry* sr, const state* s) {
        return sr_get(sr, state_hash64(s)) != SIZE_MAX;
}

void sr_insert(entry* entries, size_t cap, entry e) {
        for (size_t o = 0; o < cap; o++) {
                const size_t i = (e.hash + o) % cap;
                if (!entries[i].hash) {
                        entries[i] = e;
                        break;
                }
        }
}

void sr_grow(state_registry* sr) {
        const size_t cap = 4 * sr->cap;
        entry*       arr = calloc_(cap, sizeof(entry));
        for (size_t i = 0; i < sr->cap; i++)
                if (!arr[i].hash)
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

uint64_t sr_parent(const state_registry* sr, uint64_t hash) {
        size_t idx = sr_get(sr, hash);
        if (idx != SIZE_MAX)
                return sr->arr[idx].parent;
        return SIZE_MAX;
}
