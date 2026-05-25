#ifndef SEARCH_H
#define SEARCH_H

#include "state_registry.h"
#include <stdint.h>

#include "args.h"
#include "state.h"
#include "task.h"

#define PATH_MAX 1024

typedef struct {
        size_t len;
        size_t actions[PATH_MAX];
        size_t args[PATH_MAX][16];
} path;

path solve(const state *init, const state *goal, search_kind skind);
path solve_bfs(const state *init, const state *goal);
path solve_dfs(const state *init, const state *goal);
path solve_gbfs(const state *init, const state *goal);
path solve_lazy(const state *init, const state *goal);
path solve_beam(const state *init, const state *goal);

path trace(const state_registry *sr, const state *init, const state *goal);

void path_print(const path *p, const task *def);

#endif
