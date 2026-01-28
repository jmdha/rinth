#ifndef STATE_REGISTRY_H
#define STATE_REGISTRY_H

#include "state.h"

typedef struct state_registry state_registry;

state_registry* sr_new(void);
void            sr_free(state_registry* sr);
size_t          sr_count(const state_registry* sr);
size_t          sr_size(const state_registry* sr);
bool            sr_contains(const state_registry* sr, const state* s);
void            sr_push(state_registry* sr, const state* s);

#endif
