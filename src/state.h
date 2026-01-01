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
bool state_contains(const struct state *s, size_t predicate, size_t len, const size_t *args);

// Are states equal
bool state_equal(const struct state *a, const struct state *b);

// Is the latter state a subset
bool state_covers(const struct state *a, const struct state *b);

// How many facts from former in latter
size_t state_overlap(const struct state *a, const struct state *b);

// Is state empty
bool state_empty(const struct state *s);

// How many facts in state
size_t state_count(const struct state *s);

// Size of state in bytes
size_t state_size(const struct state *s);

// Generate a hash of state
uint64_t state_hash(const struct state *s);

// Frees state allocation
void state_free(struct state *s);

// Removes all facts from state
void state_clear(struct state *s);

// Inserts facts into state
void state_insert(struct state *s, size_t predicate, size_t len, const size_t *args);

// Removes fact from state
void state_remove(struct state *s, size_t predicate, size_t len, const size_t *args);

// Create new state iter
state_iter *state_iter_new(const struct state *s);

// Free memory of state iter
void state_iter_free(struct state_iter *si);

// Do a single step of state iter
// Returns false on no more steps
bool state_iter_step(struct state_iter *si, size_t *predicate, size_t *len,
                     size_t *args);
