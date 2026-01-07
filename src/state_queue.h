#ifndef STATE_QUEUE_H
#define STATE_QUEUE_H

#include "state.h"

typedef struct state_queue state_queue;

state_queue* sq_new(void);
void         sq_free(state_queue* sq, bool free_states);
const state* sq_front(const state_queue* sq);
const state* sq_back(const state_queue* sq);
bool         sq_empty(const state_queue* sq);
size_t       sq_size(const state_queue* sq);
void         sq_push(state_queue* sq, state* s);
state*       sq_pop(state_queue* sq);

#endif
