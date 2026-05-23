#include <stdio.h>
#include <stdlib.h>

#include "search.h"

path solve(const state* init, const state* goal, search_kind skind) {
        switch (skind) {
        case SEARCH_BFS:
                return solve_bfs(init, goal);
        case SEARCH_GBFS:
                return solve_gbfs(init, goal);
        case SEARCH_LAZY:
                return solve_lazy(init, goal);
        case SEARCH_BEAM:
                return solve_beam(init, goal);
        default:
                fprintf(stderr, "%d: unexpected search kind", skind);
                exit(1);
        }
}

path trace(const state_registry* sr, const state* init, const state* goal) {
	path p;

	return p;
}
