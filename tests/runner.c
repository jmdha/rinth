#include <criterion/criterion.h>
#include <stdio.h>

ReportHook(POST_TEST)(struct criterion_test_stats *stats) {
    printf(
        "[%12s.%-12s] finished in %f s\n", stats->test->category, stats->test->name,
        stats->elapsed_time
    );
}
