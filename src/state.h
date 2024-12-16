#pragma once

#include <stdbool.h>

#include "misc.h"

typedef struct state state; // A set of facts

state* state_new();                                              // Create an empty state
bool   state_contains(const struct state*, uint, u16*);          // Does state contain fact
bool   state_equal   (const struct state*, const struct state*); // Are states equal
bool   state_covers  (const struct state*, const struct state*); // Is the latter state a subset
bool   state_empty   (const struct state*);                      // Is state empty
uint   state_count   (const struct state*);                      // How many facts in state
uint   state_size    (const struct state*);                      // Size of state in bytes
u64    state_hash    (const struct state*);                      // Generate a hash of state
void   state_free  (struct state*);                              // Frees state allocation
void   state_insert(struct state*, uint, u16*);                   // Inserts fact into state
void   state_remove(struct state*, uint, u16*);                   // Removes fact from state
