#include <utest.h/utest.h>

#include "state.h"
#include "eval.h"

UTEST(eval, empty) {
    state *s = state_new();
    ASSERT_EQ(goal_count(s, s), 0u);
    state_free(s);
}

UTEST(eval, single) {
    state *s = state_new();
    state *g = state_new();
    state_insert(g, 0, 0, NULL);
    ASSERT_EQ(goal_count(s, g), 0u);
    state_free(s);
    state_free(g);
}

UTEST(eval, single_contain) {
    state *s = state_new();
    state *g = state_new();
    state_insert(s, 0, 0, NULL);
    state_insert(g, 0, 0, NULL);
    ASSERT_EQ(goal_count(s, g), 1u);
    state_free(s);
    state_free(g);
}
