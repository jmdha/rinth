#define STB_DS_IMPLEMENTATION
#include <stb_ds.h>

#include <criterion/criterion.h>
#include <stdio.h>

#define RESET "\033[0m"
#define RED "\033[31m"   /* Red */
#define GREEN "\033[32m" /* Green */

ReportHook(POST_TEST)(struct criterion_test_stats *stats) {
    if (stats->failed_asserts == 0 && !stats->crashed && stats->exit_code == 0) {
        printf(
            "[ %16s.%-16s ] [ %f s ] [" GREEN "   OK   " RESET "]\n", stats->test->category,
            stats->test->name, stats->elapsed_time
        );
    } else {
        printf(
            "[ %16s.%-16s ] [ %f s ] [" RED " FAILED " RESET "]\n", stats->test->category,
            stats->test->name, stats->elapsed_time
        );
    }
}
