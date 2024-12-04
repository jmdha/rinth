#include "problem.h"
#include <ubench/ubench.h>

static const char *BLOCKSWORLD = "(define (problem blocksworld-01)"
                          " (:domain blocksworld)"
                          " (:objects b1 b2)"
                          " (:init "
                          "    (arm-empty)"
                          "    (clear b2)"
                          "    (on-table b2)"
                          "    (clear b1)"
                          "    (on-table b1)"
                          " )"
                          " (:goal (and "
                          "    (clear b1)"
                          "    (on b1 b2)"
                          "    (on-table b2)"
                          ")))";

UBENCH_EX(parsing, problem) {
    Problem problem = {0};
    UBENCH_DO_BENCHMARK() {
        problem.init_count   = 0;
        problem.goal_count   = 0;
        problem.object_count = 0;
        ProblemParse(&problem, BLOCKSWORLD);
    }
}