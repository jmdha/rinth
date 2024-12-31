#pragma once

#include <stdbool.h>

#include "bounds.h"
#include "misc.h"
#include "state.h"

typedef struct path {
    uint len;
    uint actions[MAX_PATH_LEN];
    uint args[MAX_PATH_LEN][MAX_PATH_ARGS];
} path;

bool solve(path*, const state*, const state*);     // Finds a path from init to goal
bool reachable(const state*, const state*); // Whether a path exists from init to goal
