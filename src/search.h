#pragma once

#include <stdbool.h>

#include "bounds.h"
#include "misc.h"
#include "state.h"

typedef struct path {
	uint len;
	uint actions[MAX_PATH_LEN];
	uint args[MAX_PATH_LEN][MAX_PATH_ARGS];
} path_t;

// Finds path from init to goal
// len of path is set to UINT_MAX if unreachable
path_t solve(
	const state* init,
	const state* goal
);

// Whether a path exists from init to goal
bool reachable(
	const state*,
	const state* goal
);
