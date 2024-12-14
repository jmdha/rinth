#pragma once

#include <stdbool.h>

#include "fact.h"
#include "misc.h"

struct state; // A set of facts

struct state *state();                                               // Create an empty state
void state_free(struct state *s);                                    // Frees state allocation
bool state_contains(const struct state *s, struct fact *f);          // Does state contain fact
bool state_equal(const struct state *a, const struct state *b);      // Are states equal
bool state_empty(const struct state *s);                             // Is state empty
uint state_count(const struct state *s);                             // How many facts in state
uint state_size(const struct state *s);                              // Size of state in bytes
void state_insert(struct state *s, struct fact *f);                  // Inserts fact into state
void state_remove(struct state *s, struct fact *f);                  // Removes fact from state
void state_iterate(const struct state *s, void (*f)(struct fact *)); // Call on each fact
