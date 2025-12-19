#pragma once

#include "misc.h"
#include "state.h"
#include "task.h"

// Initialize eval
// Should be called once per task
void eval_init(const task_t *);

// Evaluate the state compared to the goal of task
// A high number is good
uint eval(const state *);
