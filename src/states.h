#ifndef STATES_H
#define STATES_H

#include "state.h"

typedef struct states states;

states* states_new(void);
void    states_free(states* s);
size_t  states_count(const states* s);
bool    states_contains(const states* S, const state* s);
void    states_add(states* S, state* s);
void    states_remove(states* S, state* s);
state*  states_pop(states* S);

#endif
