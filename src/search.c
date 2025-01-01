#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "log.h"
#include "search.h"
#include "bounds.h"
#include "expand.h"
#include "state.h"

#define QUEUE_LEN 100000

typedef struct node {
    uint   val;
    state* state;
} node;

static uint next(node* queue) {
    bool found = false;
    for (uint i = 0; i < QUEUE_LEN; i++)
        if (queue[i].val != UINT_MAX) {
            found = true;
            break;
        }
    if (!found) return UINT_MAX;
    while (true) {
        const uint i = rand() % QUEUE_LEN;
        if (queue[i].val != UINT_MAX)
            return i;
    }
}

bool solve(path* p, const state* init, const state* goal) {
    node queue[QUEUE_LEN];
    uint i;
    memset(queue, UINT8_MAX, QUEUE_LEN * sizeof(node));
    queue[0].val   = 0;
    queue[0].state = state_clone(init);
    while ((i = next(queue)) != UINT_MAX) {
        static u64 COUNT = 0;
        static u64 STATE_COUNT = 0;
        COUNT++;
        if (COUNT % 1000 == 0)
            INFO("%lu - %lu", COUNT, STATE_COUNT);
        node n = queue[i];
        uint action;
        u16 args[MAX_VARIABLES];
        state* child;
        queue[i].val = UINT_MAX;
        expand(n.state);
        while (expand_step(n.state, &action, args, &child)) {
            STATE_COUNT++;
            if (state_covers(child, goal))
                return true;
            const u64 hash = state_hash(child);
            if (queue[hash % QUEUE_LEN].val != UINT_MAX)
                state_free(queue[hash % QUEUE_LEN].state);
            queue[hash % QUEUE_LEN].val   = 0;
            queue[hash % QUEUE_LEN].state = child;
        }
        state_free(n.state);
    }
    return false;
}

bool reachable(const state* init, const state* goal) {
    return solve(NULL, init, goal);
}
