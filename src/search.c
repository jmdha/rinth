#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "search.h"
#include "bounds.h"
#include "expand.h"
#include "state.h"

#define QUEUE_LEN 1000

typedef struct node {
    uint   val;
    path   path;
    state* state;
} node;

static uint next(node* queue) {
    uint h_total = 0;
    for (uint i = 0; i < QUEUE_LEN; i++)
        if (queue[i].val != UINT_MAX)
            h_total += 1 + queue[i].val;
    if (h_total == 0)
        return UINT_MAX;
    uint val = rand() % h_total;
    for (uint i = 0; i < QUEUE_LEN; i++) {
        if (queue[i].val != UINT_MAX && queue[i].val <= val) {
            return i;
        }
        val -= queue[i].val;
    }
    fprintf(stderr, "Search next failed\n");
    exit(1);
}

bool solve(path* p, const state* init, const state* goal) {
    node queue[QUEUE_LEN];
    uint i;
    memset(queue, UINT8_MAX, QUEUE_LEN * sizeof(node));
    queue[0].val   = 0;
    queue[0].state = state_clone(init);
    while ((i = next(queue)) != UINT_MAX) {
        node n = queue[i];
        uint action;
        u16 args[MAX_VARIABLES];
        state* child;
        queue[i].val = UINT_MAX;
        expand(n.state);
        while (expand_step(n.state, &action, args, &child)) {
            if (state_covers(child, goal)) {
                if (p)
                    memcpy(p, &n.path, sizeof(path));
                return true;
            }
            const u64 hash = state_hash(child);
            queue[hash % QUEUE_LEN].val   = 0;
            queue[hash % QUEUE_LEN].state = child;
            memcpy(&queue[hash % QUEUE_LEN].path, &n.path, sizeof(path));
        }
    }
    return false;
}

bool reachable(const state* init, const state* goal) {
    return solve(NULL, init, goal);
}
