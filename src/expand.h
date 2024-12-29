#pragma once

#include "task.h"

void expand_init(const struct task*);
void expand(const state*);
bool expand_step(uint*, uint*, uint*);
uint expand_count();
