#include "problem.h"
#include <criterion/assert.h>
#include <criterion/internal/assert.h>
#include <criterion/parameterized.h>

Test(parse_problem, empty) {
    const char *str = "(define"
                      ")";

    Problem problem = ProblemParse(str);

    cr_assert_eq(problem.name, NULL);
    cr_assert_eq(problem.domain, NULL);
    cr_assert_eq(problem.object_count, 0);
    cr_assert_eq(problem.init_count, 0);
    cr_assert_eq(problem.goal_count, 0);

    ProblemDelete(&problem);
}

Test(parse_problem, name) {
    const char *str = "(define"
                      "    (problem a)"
                      ")";

    Problem problem = ProblemParse(str);

    cr_assert_str_eq(problem.name, "a");
    cr_assert_eq(problem.domain, NULL);
    cr_assert_eq(problem.object_count, 0);
    cr_assert_eq(problem.init_count, 0);
    cr_assert_eq(problem.goal_count, 0);

    ProblemDelete(&problem);
}

Test(parse_problem, domain) {
    const char *str = "(define"
                      "    (:domain a)"
                      ")";

    Problem problem = ProblemParse(str);

    cr_assert_eq(problem.name, NULL);
    cr_assert_str_eq(problem.domain, "a");
    cr_assert_eq(problem.object_count, 0);
    cr_assert_eq(problem.init_count, 0);
    cr_assert_eq(problem.goal_count, 0);

    ProblemDelete(&problem);
}

Test(parse_problem, objects) {
    const char *str = "(define"
                      "    (:objects a bb c-_)"
                      ")";

    Problem problem = ProblemParse(str);

    cr_assert_eq(problem.name, NULL);
    cr_assert_eq(problem.domain, NULL);
    cr_assert_eq(problem.object_count, 3);
    cr_assert_eq(problem.init_count, 0);
    cr_assert_eq(problem.goal_count, 0);

    cr_assert_str_eq(problem.objects[0], "a");
    cr_assert_str_eq(problem.objects[1], "bb");
    cr_assert_str_eq(problem.objects[2], "c-_");

    ProblemDelete(&problem);
}
