#include "expand.h"
#include "search.h"
#include "state_queue.h"

path solve_bfs(const state* init, const state* goal) {
        path            p;
        size_t          action;
        size_t          args[64];
        state*          node;
        state*          child;
        state_queue*    queue;
        state_registry* visited;

        queue   = sq_new();
        visited = sr_new();
        sq_push(queue, state_clone(init));
        sr_push(visited, init, NULL);

        while ((node = sq_pop(queue))) {
                expand(node);
                while ((child = successor(&action, args))) {
                        if (sr_contains(visited, child)) {
                                state_free(child);
                                continue;
                        }
                        sr_push(visited, child, node);

                        if (state_covers(child, goal)) {
                                p = trace(visited, init, child);
                                state_free(node);
                                state_free(child);
                                sq_free(queue, true);
                                sr_free(visited);
                                return p;
                        }
                        sq_push(queue, child);
                }
                state_free(node);
        }
        sq_free(queue, true);
        p.len = SIZE_MAX;
        return p;
}
