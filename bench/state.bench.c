#include <ubench/ubench.h>

#include "state.h"

struct fact random_fact() {
    const uint pred  = rand() % 4;
    struct fact fact = {.predicate = pred, .len = pred};
    for (uint t = 0; t < pred; t++)
        fact.args[t] = rand() % 1000;
    return fact;
}

UBENCH_EX(state, contains) {
    struct state *s = state();
    for (uint i = 0; i < 1000; i++) {
        struct fact fact = random_fact();
        state_insert(s, &fact);
    }
    UBENCH_DO_BENCHMARK() {
        for (uint i = 0; i < 100; i++) {
            struct fact fact = random_fact();
            state_contains(s, &fact);
        }
    }
    free(s);
}

UBENCH(state, random_insert) {
    struct state *s = state();
    for (uint i = 0; i < 1000; i++) {
        struct fact fact = random_fact();
        state_insert(s, &fact);
    }
    free(s);
}
