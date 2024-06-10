#include "munit.h"

static MunitResult test_temp(const MunitParameter params[], void *data) {
    munit_assert_int(temp_function(), ==, 4);
    return MUNIT_OK;
}

static MunitTest test_suite_tests[] = {
    {(char *)"temp", test_temp, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};

static const MunitSuite test_suite = {
    (char *)"", test_suite_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char *argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
    return munit_suite_main(&test_suite, NULL, argc, argv);
}
