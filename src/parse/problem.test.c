#include <criterion/criterion.h>

#include "parse/problem.h"

Test(parse_problem, empty) {
    const char *str = "(define"
                      ")";

    Problem problem = {0};
    ProblemParse(&problem, str);

    cr_assert_eq(problem.name.ptr, NULL);
    cr_assert_eq(problem.domain.ptr, NULL);
    cr_assert_eq(problem.object_count, 0);
    cr_assert_eq(problem.init_count, 0);
    cr_assert_eq(problem.goal_count, 0);
}

Test(parse_problem, name) {
    const char *str = "(define"
                      "    (problem a)"
                      ")";

    Problem problem = {0};
    ProblemParse(&problem, str);

    cr_assert(strncmp(problem.name.ptr, "a", problem.name.len) == 0);
    cr_assert_eq(problem.domain.ptr, NULL);
    cr_assert_eq(problem.object_count, 0);
    cr_assert_eq(problem.init_count, 0);
    cr_assert_eq(problem.goal_count, 0);
}

Test(parse_problem, domain) {
    const char *str = "(define"
                      "    (:domain a)"
                      ")";

    Problem problem = {0};
    ProblemParse(&problem, str);

    cr_assert_eq(problem.name.ptr, NULL);
    cr_assert(strncmp(problem.domain.ptr, "a", problem.domain.len) == 0);
    cr_assert_eq(problem.object_count, 0);
    cr_assert_eq(problem.init_count, 0);
    cr_assert_eq(problem.goal_count, 0);
}

Test(parse_problem, objects) {
    const char *str = "(define"
                      "    (:objects a bb c-_)"
                      ")";

    Problem problem = {0};
    ProblemParse(&problem, str);

    cr_assert_eq(problem.name.ptr, NULL);
    cr_assert_eq(problem.domain.ptr, NULL);
    cr_assert_eq(problem.object_count, 3);
    cr_assert_eq(problem.init_count, 0);
    cr_assert_eq(problem.goal_count, 0);

    cr_assert(strncmp(problem.objects[0].ptr, "a", problem.objects[0].len) == 0);
    cr_assert(strncmp(problem.objects[1].ptr, "bb", problem.objects[1].len) == 0);
    cr_assert(strncmp(problem.objects[2].ptr, "c-_", problem.objects[2].len) == 0);
}
