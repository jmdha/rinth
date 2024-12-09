#include <ubench/ubench.h>

#include "state.h"

UBENCH(state, random_insert) {
    struct state *s = state();
    for (uint i = 0; i < 1000; i++) {
        const uint pred = rand() % 4;
        struct fact fact = {.predicate = pred, .len = pred};
        for (uint t = 0; t < pred; t++)
            fact.args[t] = rand() % 1000;
        state_insert(s, &fact);
    }
    free(s);
}
