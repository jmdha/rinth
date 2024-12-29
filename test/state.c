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

UTEST(state, covers) {
    struct state *s1 = state_new();
    struct state *s2 = state_new();
    state_insert(s1, 0, 0, NULL);
    state_insert(s1, 1, 0, NULL);
    state_insert(s2, 0, 0, NULL);
    ASSERT_TRUE(state_covers(s1, s2));
    ASSERT_TRUE(!state_covers(s2, s1));
    state_free(s1);
    state_free(s2);
}

UTEST(state, insert_contains) {
    struct state *s = state_new();
    ASSERT_TRUE(!state_contains(s, 0, 0, NULL));
    state_insert(s, 0, 0, NULL);
    ASSERT_TRUE(state_contains(s, 0, 0, NULL));
    state_free(s);
}

UTEST(state, insert_contains_args) {
    struct state *s = state_new();
    u16 args[] = {0};
    ASSERT_TRUE(!state_contains(s, 0, 1, args));
    state_insert(s, 0, 1, args);
    ASSERT_TRUE(state_contains(s, 0, 1, args));
    state_free(s);
}

UTEST(state, remove_contains) {
    struct state *s = state_new();
    state_insert(s, 0, 0, NULL);
    ASSERT_TRUE(state_contains(s, 0, 0, NULL));
    state_remove(s, 0, 0, NULL);
    ASSERT_TRUE(!state_contains(s, 0, 0, NULL));
    state_free(s);
}

UTEST(state, remove_contains_args) {
    struct state *s = state_new();
    u16 args[] = {1};
    state_insert(s, 0, 1, args);
    ASSERT_TRUE(state_contains(s, 0, 1, args));
    state_remove(s, 0, 1, args);
    ASSERT_TRUE(!state_contains(s, 0, 1, args));
    state_free(s);
}

UTEST(state, multi_insert_remove) {
    struct state *s = state_new();
    u16 args1[] = {0};
    u16 args2[] = {0, 0};
    state_insert(s, 0, 0, NULL);
    state_insert(s, 1, 1, args1);
    state_insert(s, 2, 2, args2);
    ASSERT_TRUE(state_contains(s, 0, 0, NULL));
    ASSERT_TRUE(state_contains(s, 1, 1, args1));
    ASSERT_TRUE(state_contains(s, 2, 2, args2));
    state_remove(s, 0, 0, NULL);
    ASSERT_TRUE(!state_contains(s, 0, 0, NULL));
    ASSERT_TRUE(state_contains(s, 1, 1, args1));
    ASSERT_TRUE(state_contains(s, 2, 2, args2));
    state_remove(s, 1, 1, args1);
    ASSERT_TRUE(!state_contains(s, 0, 0, NULL));
    ASSERT_TRUE(!state_contains(s, 1, 1, args1));
    ASSERT_TRUE(state_contains(s, 2, 2, args2));
    state_remove(s, 2, 2, args2);
    ASSERT_TRUE(!state_contains(s, 0, 0, NULL));
    ASSERT_TRUE(!state_contains(s, 1, 1, args1));
    ASSERT_TRUE(!state_contains(s, 2, 2, args2));
    state_free(s);
}

UTEST(state, equal) {
    struct state *s1 = state_new();
    struct state *s2 = state_new();
    u16 args0[] = {0};
    u16 args1[] = {0};
    state_insert(s1, 0, 1, args0);
    ASSERT_TRUE(!state_equal(s1, s2));
    state_insert(s2, 0, 1, args0);
    ASSERT_TRUE(state_equal(s1, s2));
    state_insert(s2, 1, 1, args1);
    ASSERT_TRUE(!state_equal(s1, s2));
    state_free(s1);
    state_free(s2);
}
