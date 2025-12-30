#ifndef EXPAND_H
#define EXPAND_H

#include "task.h"
#include "state.h"

void expand_init(const task* t);
void expand_init_cp(const task* t);
void expand_init_sqlite(const task* t);

void expand(const state* s);
void expand_cp(const state* s);
void expand_sqlite(const state* s);

bool expand_step(size_t* action, size_t* args);
bool expand_step_cp(size_t* action, size_t* args);
bool expand_step_sqlite(size_t* action, size_t* args);

state* successor(size_t* action, size_t* args);

#endif
