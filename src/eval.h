#pragma once

#include "task.h"
#include "args.h"

void eval_init(const task* def, eval_kind ekind);
void eval_init_goal_count(const task* def);
void eval_init_hadd(const task* def);
void eval_init_hff(const task* def);
void eval_init_hmax(const task* def);

size_t eval(const state* s);
size_t eval_goal_count(const state* s);
size_t eval_hadd(const state* s);
size_t eval_hff(const state* s);
size_t eval_hmax(const state* s);
