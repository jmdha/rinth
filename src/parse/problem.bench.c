#include <ubench/ubench.h>

#include "problem.h"

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
    Problem problem;
    UBENCH_DO_BENCHMARK() {
        for (uint i = 0; i < 1000; i++)
            problem_parse_(&problem, BLOCKSWORLD);
    }
}
