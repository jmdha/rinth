#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "misc.h"

typedef struct state state;           // A set of facts
typedef struct state_iter state_iter; // An iterator of facts

// Create an empty state
state *state_new(void);

// Clone a state
state *state_clone(const state *s);

// Does state contain fact
bool state_contains(const state *s, size_t predicate, size_t len, const size_t *args);

// Are states equal
bool state_equal(const state *a, const state *b);

// Is the latter state a subset
bool state_covers(const state *a, const state *b);

// How many facts from former in latter
size_t state_overlap(const state *a, const state *b);

// Is state empty
bool state_empty(const state *s);

// How many facts in state
size_t state_count(const state *s);

// Size of state in bytes
size_t state_size(const state *s);

// Bits per Fact
// How many bits are used to store a fact on average
float state_bpf(const state* s);

// Generate a hash of state
uint64_t state_hash(const state *s);

// Frees state allocation
void state_free(state *s);

// Removes all facts from state
void state_clear(state *s);

// Inserts facts into state
void state_insert(state *s, size_t predicate, size_t len, const size_t *args);

// Removes fact from state
void state_remove(state *s, size_t predicate, size_t len, const size_t *args);

// Create new state iter
state_iter *state_iter_new(const state *s);

// Free memory of state iter
void state_iter_free(state_iter *si);

// Do a single step of state iter
// Returns false on no more steps
bool state_iter_step(state_iter *si, size_t *predicate, size_t *len,
                     size_t *args);
