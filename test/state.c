#include <utest.h/utest.h>

#include "state.h"

UTEST(state, empty) {
    struct state *s = state_new();
    ASSERT_EQ(state_count(s), 0u);
    ASSERT_TRUE(state_empty(s));
    state_free(s);
}

UTEST(state, empty_equal_empty) {
    struct state *s1 = state_new();
    struct state *s2 = state_new();
    ASSERT_TRUE(state_equal(s1, s2));
    state_free(s1);
    state_free(s2);
}

UTEST(state, insert_contains) {
    struct state *s = state_new();
    u16 fact[] = {1};
    ASSERT_TRUE(!state_contains(s, 1, fact));
    state_insert(s, 1, fact);
    ASSERT_TRUE(state_contains(s, 1, fact));
    state_free(s);
}

UTEST(state, insert_contains_args) {
    struct state *s = state_new();
    u16 fact[] = {1, 2};
    ASSERT_TRUE(!state_contains(s, 2, fact));
    state_insert(s, 2, fact);
    ASSERT_TRUE(state_contains(s, 2, fact));
    state_free(s);
}

UTEST(state, remove_contains) {
    struct state *s = state_new();
    u16 fact[] = {1};
    state_insert(s, 1, fact);
    ASSERT_TRUE(state_contains(s, 1, fact));
    state_remove(s, 1, fact);
    ASSERT_TRUE(!state_contains(s, 1, fact));
    state_free(s);
}

UTEST(state, remove_contains_args) {
    struct state *s = state_new();
    u16 fact[] = {1, 2};
    state_insert(s, 2, fact);
    ASSERT_TRUE(state_contains(s, 2, fact));
    state_remove(s, 2, fact);
    ASSERT_TRUE(!state_contains(s, 2, fact));
    state_free(s);
}

UTEST(state, multi_insert_remove) {
    struct state *s = state_new();
    u16 fact0[] = {1};
    u16 fact1[] = {2, 1};
    u16 fact2[] = {3, 1, 2};
    state_insert(s, 1, fact0);
    state_insert(s, 2, fact1);
    state_insert(s, 3, fact2);
    ASSERT_TRUE(state_contains(s, 1, fact0));
    ASSERT_TRUE(state_contains(s, 2, fact1));
    ASSERT_TRUE(state_contains(s, 3, fact2));
    state_remove(s, 1, fact0);
    ASSERT_TRUE(!state_contains(s, 1, fact0));
    ASSERT_TRUE(state_contains(s, 2, fact1));
    ASSERT_TRUE(state_contains(s, 3, fact2));
    state_remove(s, 2, fact1);
    ASSERT_TRUE(!state_contains(s, 1, fact0));
    ASSERT_TRUE(!state_contains(s, 2, fact1));
    ASSERT_TRUE(state_contains(s, 3, fact2));
    state_remove(s, 3, fact2);
    ASSERT_TRUE(!state_contains(s, 1, fact0));
    ASSERT_TRUE(!state_contains(s, 2, fact1));
    ASSERT_TRUE(!state_contains(s, 3, fact2));
    state_free(s);
}

UTEST(state, equal) {
    struct state *s1 = state_new();
    struct state *s2 = state_new();
    u16 fact1[] = {1};
    u16 fact2[] = {2};
    state_insert(s1, 1, fact1);
    ASSERT_TRUE(!state_equal(s1, s2));
    state_insert(s2, 1, fact1);
    ASSERT_TRUE(state_equal(s1, s2));
    state_insert(s2, 1, fact2);
    ASSERT_TRUE(!state_equal(s1, s2));
    state_free(s1);
    state_free(s2);
}
