#include <utest.h/utest.h>

#include "algo.h"

UTEST(algo_cp, empty) {
        size_t* vals[] = {NULL};
        size_t  out[]  = {0};

        cp_iter* cpi = cp_init(vals);

        ASSERT_FALSE(cp_step(out, cpi));
}

UTEST(algo_cp, column_single) {
        size_t  vals_a[] = {1, SIZE_MAX};
        size_t* vals[]   = {vals_a, NULL};
        size_t  out[]    = {0};

        cp_iter* cpi = cp_init(vals);

        ASSERT_TRUE(cp_step(out, cpi));
        ASSERT_EQ(1, out[0]);
        ASSERT_FALSE(cp_step(out, cpi));
}

UTEST(algo_cp, column_multiple) {
        size_t  vals_a[] = {1, 2, SIZE_MAX};
        size_t* vals[]   = {vals_a, NULL};
        size_t  out[]    = {0};

        cp_iter* cpi = cp_init(vals);

        ASSERT_TRUE(cp_step(out, cpi));
        ASSERT_EQ(1, out[0]);
        ASSERT_TRUE(cp_step(out, cpi));
        ASSERT_EQ(2, out[0]);
        ASSERT_FALSE(cp_step(out, cpi));
}

UTEST(algo_cp, 2_columns) {
        size_t  vals_a[] = {1, SIZE_MAX};
        size_t  vals_b[] = {10, SIZE_MAX};
        size_t* vals[]   = {vals_a, vals_b, NULL};
        size_t  out[]    = {0, 0};

        cp_iter* cpi = cp_init(vals);

        ASSERT_TRUE(cp_step(out, cpi));
        ASSERT_EQ(1, out[0]);
        ASSERT_EQ(10, out[1]);
        ASSERT_FALSE(cp_step(out, cpi));
}

UTEST(algo_cp, 2_columns_multiple) {
        size_t  vals_a[] = {1, 2, SIZE_MAX};
        size_t  vals_b[] = {10, 11, SIZE_MAX};
        size_t* vals[]   = {vals_a, vals_b, NULL};
        size_t  out[]    = {0, 0};

        cp_iter* cpi = cp_init(vals);

        ASSERT_TRUE(cp_step(out, cpi));
        ASSERT_EQ(1, out[0]);
        ASSERT_EQ(10, out[1]);
        ASSERT_TRUE(cp_step(out, cpi));
        ASSERT_EQ(2, out[0]);
        ASSERT_EQ(10, out[1]);
        ASSERT_TRUE(cp_step(out, cpi));
        ASSERT_EQ(1, out[0]);
        ASSERT_EQ(11, out[1]);
        ASSERT_TRUE(cp_step(out, cpi));
        ASSERT_EQ(2, out[0]);
        ASSERT_EQ(11, out[1]);
        ASSERT_FALSE(cp_step(out, cpi));
}

UTEST(algo_cp, 3_columns_multiple) {
        size_t  vals_a[] = {1, SIZE_MAX};
        size_t  vals_b[] = {11, 12, SIZE_MAX};
        size_t  vals_c[] = {21, 22, 33, SIZE_MAX};
        size_t* vals[]   = {vals_a, vals_b, vals_c, NULL};
        size_t  out[]    = {0, 0, 0};

        cp_iter* cpi = cp_init(vals);

        ASSERT_TRUE(cp_step(out, cpi));
        ASSERT_EQ(1, out[0]);
        ASSERT_EQ(11, out[1]);
        ASSERT_EQ(21, out[2]);
        ASSERT_TRUE(cp_step(out, cpi));
        ASSERT_EQ(1, out[0]);
        ASSERT_EQ(12, out[1]);
        ASSERT_EQ(21, out[2]);
        ASSERT_TRUE(cp_step(out, cpi));
        ASSERT_EQ(1, out[0]);
        ASSERT_EQ(11, out[1]);
        ASSERT_EQ(22, out[2]);
        ASSERT_TRUE(cp_step(out, cpi));
        ASSERT_EQ(1, out[0]);
        ASSERT_EQ(12, out[1]);
        ASSERT_EQ(22, out[2]);
        ASSERT_TRUE(cp_step(out, cpi));
        ASSERT_EQ(1, out[0]);
        ASSERT_EQ(11, out[1]);
        ASSERT_EQ(33, out[2]);
        ASSERT_TRUE(cp_step(out, cpi));
        ASSERT_EQ(1, out[0]);
        ASSERT_EQ(12, out[1]);
        ASSERT_EQ(33, out[2]);
        ASSERT_FALSE(cp_step(out, cpi));
}
