#ifndef STATE_SET_H
#define STATE_SET_H

#include "state.h"

typedef struct state_set state_set;

state_set* ss_new(void);
void       ss_free(state_set* ss);
size_t     ss_count(const state_set* ss);
bool       ss_empty(const state_set* ss);
bool       ss_contains(const state_set* ss, const state* s);
void       ss_add(state_set* ss, state* s);
void       ss_remove(state_set* ss, state* s);
state*     ss_pop(state_set* ss);

#endif
