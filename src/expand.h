#pragma once

#include "task.h"

void expand_init(const struct task*);
void expand(const state*);
bool expand_step(const state*, uint*, u16*, state**);
uint expand_count(const state*);
