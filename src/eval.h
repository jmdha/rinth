#pragma once

#include "misc.h"
#include "state.h"
#include "task.h"

void eval_init(const struct task*);
uint eval(const state*);
uint goal_count(const state*, const state*);
