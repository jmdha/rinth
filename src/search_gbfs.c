#include <stdio.h>
#include <stdlib.h>

#include "eval.h"
#include "expand.h"
#include "log.h"
#include "search.h"
#include "state_heap.h"
#include "state_registry.h"

path solve_gbfs(const state* init, const state* goal) {
        path            p;
        size_t          action;
        size_t          args[64];
        state_registry* visited;
        state_heap*     queue;
        state*          node;
        state*          child;

        visited = sr_new();
        queue   = sh_new();
        sh_push(queue, state_clone(init), 0);
        sr_push(visited, init, NULL);

        while ((node = sh_pop(queue)) != NULL) {
                expand(node);
                while ((child = successor(&action, args))) {
                        if (sr_contains(visited, child)) {
                                state_free(child);
                                continue;
                        }
                        sr_push(visited, child, node);

                        if (state_covers(child, goal)) {
                                INFO("SH: %zu %zu B", sh_count(queue), sh_size(queue));
                                p = trace(visited, init, child);
                                state_free(child);
                                state_free(node);
                                sr_free(visited);
                                sh_free(queue);
                                return p;
                        }

                        sh_push(queue, child, eval(child));
                }
                state_free(node);
        }

        p.len = SIZE_MAX;
        return p;
}
