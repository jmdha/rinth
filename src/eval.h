#pragma once

#include "task.h"
#include "args.h"

void eval_init(const task* def, eval_kind ekind);
void eval_init_goal_count(const task* def, eval_kind ekind);

size_t eval(const state* s);
size_t eval_goal_count(const state* s);
