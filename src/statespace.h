#pragma once

#include "state.h"

typedef struct statespace statespace;
typedef struct statespace_iter statespace_iter;

statespace* statespace_new();
size_t      statespace_count(const statespace*);
bool        statespace_contains(const statespace*, const state*);
void        statespace_add(statespace*, state*);
