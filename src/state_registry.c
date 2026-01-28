#include <errno.h>
#include <stdlib.h>
#include <memory.h>

#include "eval.h"
#include "state_registry.h"

typedef struct {
	uint64_t hash64;
	uint32_t hash32;
	uint16_t hash16;
	uint16_t eval;
} entry;

static entry entry_new(const state* s) {
	entry e  = { 0 };
	e.hash64 = state_hash64(s);
	e.hash32 = state_hash32(s);
	e.hash16 = state_hash16(s);
	e.eval   = eval(s);
	return e;
}

static bool is_entry(const entry* e) {
	return e->hash64 || e->hash32 || e->hash16 || e->eval;
}

static bool entry_equal(const entry* a, const entry* b) {
	return memcmp(a, b, sizeof(entry)) == 0;
}

static uint64_t entry_hash(const entry* e) {
	return e->hash64;
}

struct state_registry {
	size_t len;
	size_t cap;
	entry* arr;
};

state_registry* sr_new(void) {
	state_registry* sr = malloc(sizeof(state_registry));
	if (!sr)
		exit(errno);
	sr->len = 0;
	sr->cap = 8;
	sr->arr = calloc(8, sizeof(entry));
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
	entry    e    = entry_new(s);
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
	entry*       arr = calloc(cap, sizeof(entry));
	if (!arr)
		exit(errno);
	for (size_t i = 0; i < sr->cap; i++)
		if (!is_entry(&arr[i]))
			sr_insert(arr, cap, sr->arr[i]);
	free(sr->arr);
	sr->cap = cap;
	sr->arr = arr;
}

void sr_push(state_registry* sr, const state* s) {
	if (4 * sr->len > sr->cap)
		sr_grow(sr);
	sr_insert(sr->arr, sr->cap, entry_new(s));
	sr->len++;
}
