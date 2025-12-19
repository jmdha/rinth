#pragma once

#include "task.h"

void expand_init(const task_t *task);

void expand(const state *s);

bool expand_step(const state *s, uint *predicate, u16 *len, state **args);

uint expand_count(const state *s);
