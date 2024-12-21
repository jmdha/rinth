#pragma once

#include "task.h"

void expand_init(const struct task*);
void expand(const state*, void (*)(uint, state*));
uint expand_count(const state*);
