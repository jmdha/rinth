#include <criterion/criterion.h>

#include "translate/state.h"

Test(state, empty) {
    struct state *s = state();
    cr_assert_eq(state_count(s), 0);
    cr_assert(state_empty(s));
    state_free(s);
}

Test(state, empty_equal_empty) {
    struct state *s1 = state();
    struct state *s2 = state();
    cr_assert(state_equal(s1, s2));
    state_free(s1);
    state_free(s2);
}

Test(state, insert_contains) {
    struct state *s = state();
    struct fact f = {.predicate = 0, .len = 0};
    cr_assert(!state_contains(s, &f));
    state_insert(s, &f);
    cr_assert(state_contains(s, &f));
    state_free(s);
}

Test(state, insert_contains_args) {
    struct state *s = state();
    struct fact f = {.predicate = 0, .len = 3, .args = {0, 0, 0}};
    cr_assert(!state_contains(s, &f));
    state_insert(s, &f);
    cr_assert(state_contains(s, &f));
    state_free(s);
}

Test(state, remove_contains) {
    struct state *s = state();
    struct fact f = {.predicate = 0, .len = 0};
    state_insert(s, &f);
    cr_assert(state_contains(s, &f));
    state_remove(s, &f);
    cr_assert(!state_contains(s, &f));
    state_free(s);
}

Test(state, remove_contains_args) {
    struct state *s = state();
    struct fact f = {.predicate = 0, .len = 3, .args = {0, 0, 0}};
    state_insert(s, &f);
    cr_assert(state_contains(s, &f));
    state_remove(s, &f);
    cr_assert(!state_contains(s, &f));
    state_free(s);
}

Test(state, multi_insert_remove) {
    struct state *s = state();
    struct fact f1 = {.predicate = 0, .len = 0};
    struct fact f2 = {.predicate = 1, .len = 1, .args = {1}};
    struct fact f3 = {.predicate = 2, .len = 2, .args = {2, 0}};
    state_insert(s, &f1);
    state_insert(s, &f2);
    state_insert(s, &f3);
    cr_assert(state_contains(s, &f1));
    cr_assert(state_contains(s, &f2));
    cr_assert(state_contains(s, &f3));
    state_remove(s, &f1);
    cr_assert(!state_contains(s, &f1));
    cr_assert(state_contains(s, &f2));
    cr_assert(state_contains(s, &f3));
    state_remove(s, &f2);
    cr_assert(!state_contains(s, &f1));
    cr_assert(!state_contains(s, &f2));
    cr_assert(state_contains(s, &f3));
    state_remove(s, &f3);
    cr_assert(!state_contains(s, &f1));
    cr_assert(!state_contains(s, &f2));
    cr_assert(!state_contains(s, &f3));
    state_free(s);
}

Test(state, equal) {
    struct state *s1 = state();
    struct state *s2 = state();
    struct fact f1 = {.predicate = 0, .len = 0};
    struct fact f2 = {.predicate = 1, .len = 0};
    state_insert(s1, &f1);
    cr_assert(!state_equal(s1, s2));
    state_insert(s2, &f1);
    cr_assert(state_equal(s1, s2));
    state_insert(s2, &f2);
    cr_assert(!state_equal(s1, s2));
    state_free(s1);
    state_free(s2);
}
