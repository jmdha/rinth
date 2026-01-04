#ifndef SEARCH_H
#define SEARCH_H

#include <stdint.h>

#include "args.h"
#include "state.h"

typedef struct {
	size_t len;
	size_t actions[1024];
	size_t args[1024][16];
} path;

path solve(const state* init, const state* goal, search_kind skind);
path solve_bfs(const state* init, const state* goal);
path solve_dfs(const state* init, const state* goal);
path solve_gbfs(const state* init, const state* goal);
path solve_lazy(const state* init, const state* goal);

#endif
