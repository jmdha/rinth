#include <stdio.h>
#include <stdlib.h>

#include "expand.h"
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
        // TRACE PATH
        path     out = {0};
        uint64_t phash[PATH_MAX];
        phash[out.len++] = state_hash64(goal);
        for (size_t i = 0; i <= PATH_MAX; i++) {
                if (i == PATH_MAX) {
                        fprintf(stderr, "trace reached PATH_MAX\n");
                        exit(1);
                }
                phash[out.len] = sr_parent(sr, phash[out.len - 1]);
                if (!phash[out.len])
                        break;
                out.len++;
        }
        out.len--;
        // REVERSE PATH
        for (size_t i = 0; i < out.len / 2; i++) {
                uint64_t tmp           = phash[i];
                phash[i]               = phash[out.len - i - 1];
                phash[out.len - i - 1] = tmp;
        }
        // REGENERATE PATH
        state* tmp = state_clone(init);
        for (size_t i = 0; i < out.len; i++) {
                expand(tmp);
                state* child;
                bool   found = false;
                while ((child = successor(&out.actions[i], out.args[i]))) {
                        if (state_hash64(child) == phash[i]) {
                                found = true;
                                break;
                        }
                }
                if (!found) {
                        fprintf(stderr, "trace could not regenerate path\n");
                        exit(1);
                }
                tmp = child;
        }
        return out;
}

void path_print(const path* p, const task* def) {
        for (size_t i = 0; i < p->len; i++) {
                printf("%.*s", (int)def->actions[p->actions[i]].name.len,
                       def->actions[p->actions[i]].name.ptr);
                for (size_t t = 0; t < def->actions[p->actions[i]].arity; t++)
                        printf(" %.*s", (int)def->objects[p->args[i][t]].len,
                               def->objects[p->args[i][t]].ptr);
                printf("\n");
        }
}
