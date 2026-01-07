#ifndef STATE_HEAP_H
#define STATE_HEAP_H

#include "state.h"

typedef struct state_heap state_heap;

state_heap* sh_new(void);
void        sh_free(state_heap* sh);
bool        sh_empty(const state_heap* sh);
size_t      sh_size(const state_heap* sh);
void        sh_push(state_heap* sh, state* s, size_t val);
state*      sh_pop(state_heap* sh);

#endif
