#include <ubench/ubench.h>

#include "state.h"

uint random_fact(u16* args) {
    const uint pred  = rand() % 4;
    args[0] = pred;
    for (uint t = 1; t <= pred; t++)
        args[t] = rand() % 1000;
    return pred;
}

UBENCH_EX(state, contains) {
    struct state *s = state_new();
    u16 buffer[1000];
    for (uint i = 0; i < 1000; i++) {
        uint len = random_fact(buffer);
        state_insert(s, len, buffer);
    }
    UBENCH_DO_BENCHMARK() {
        for (uint i = 0; i < 100; i++) {
            uint len = random_fact(buffer);
            state_contains(s, len, buffer);
        }
    }
    state_free(s);
}

UBENCH(state, random_insert) {
    struct state *s = state_new();
    u16 buffer[4];
    for (uint i = 0; i < 1000; i++) {
        uint len = random_fact(buffer);
        state_insert(s, len, buffer);
    }
    state_free(s);
}
