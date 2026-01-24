#include <utest.h/utest.h>

#include "state.h"

UTEST(state, empty) {
	state* s = state_new();

	ASSERT_EQ(0, state_count(s));

	state_free(s);
}

UTEST(state, contains_nullary) {
	state* s = state_new();

	state_insert(s, 0, 0, NULL);
	state_insert(s, 1, 0, NULL);
	state_insert(s, 4, 0, NULL);

	ASSERT_EQ(3, state_count(s));
	ASSERT_TRUE(state_contains(s, 0, 0, NULL));
	ASSERT_TRUE(state_contains(s, 1, 0, NULL));
	ASSERT_FALSE(state_contains(s, 2, 0, NULL));
	ASSERT_FALSE(state_contains(s, 3, 0, NULL));
	ASSERT_TRUE(state_contains(s, 4, 0, NULL));

	state_remove(s, 0, 0, NULL);
	state_remove(s, 1, 0, NULL);
	state_remove(s, 4, 0, NULL);

	ASSERT_EQ(0, state_count(s));
	ASSERT_FALSE(state_contains(s, 0, 0, NULL));
	ASSERT_FALSE(state_contains(s, 1, 0, NULL));
	ASSERT_FALSE(state_contains(s, 2, 0, NULL));
	ASSERT_FALSE(state_contains(s, 3, 0, NULL));
	ASSERT_FALSE(state_contains(s, 4, 0, NULL));

	state_free(s);
}

UTEST(state, contains_unary) {
	state* s = state_new();

	size_t vals[] = { 0, 1, 2, 3, 4 };

	state_insert(s, 0, 1, &vals[0]);
	state_insert(s, 0, 1, &vals[1]);
	state_insert(s, 0, 1, &vals[4]);
	state_insert(s, 1, 1, &vals[0]);
	state_insert(s, 1, 1, &vals[1]);
	state_insert(s, 1, 1, &vals[4]);

	ASSERT_EQ(6, state_count(s));
	ASSERT_TRUE(state_contains(s, 0, 1, &vals[0]));
	ASSERT_TRUE(state_contains(s, 0, 1, &vals[1]));
	ASSERT_FALSE(state_contains(s, 0, 1, &vals[2]));
	ASSERT_FALSE(state_contains(s, 0, 1, &vals[3]));
	ASSERT_TRUE(state_contains(s, 0, 1, &vals[4]));
	ASSERT_TRUE(state_contains(s, 1, 1, &vals[0]));
	ASSERT_TRUE(state_contains(s, 1, 1, &vals[1]));
	ASSERT_FALSE(state_contains(s, 1, 1, &vals[2]));
	ASSERT_FALSE(state_contains(s, 1, 1, &vals[3]));
	ASSERT_TRUE(state_contains(s, 1, 1, &vals[4]));

	state_free(s);
}

UTEST(state, equal) {
	state* a = state_new();
	state* b = state_new();

	ASSERT_TRUE(state_equal(a, b));

	state_insert(a, 0, 0, NULL);

	ASSERT_FALSE(state_equal(a, b));

	state_insert(b, 0, 0, NULL);

	ASSERT_TRUE(state_equal(a, b));

	state_free(a);
	state_free(b);
}
