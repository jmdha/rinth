#include <utest.h/utest.h>

#include "parse/problem.h"

UTEST(parse_problem, empty) {
    const char *str = "(define"
                      ")";

    Problem problem = {0};
    ProblemParse(&problem, str);

    ASSERT_EQ(problem.name.ptr, NULL);
    ASSERT_EQ(problem.domain.ptr, NULL);
    ASSERT_EQ(problem.object_count, 0u);
    ASSERT_EQ(problem.init_count, 0u);
    ASSERT_EQ(problem.goal_count, 0u);
}

UTEST(parse_problem, name) {
    const char *str = "(define"
                      "    (problem a)"
                      ")";

    Problem problem = {0};
    ProblemParse(&problem, str);

    ASSERT_TRUE(strncmp(problem.name.ptr, "a", problem.name.len) == 0);
    ASSERT_EQ(problem.domain.ptr, NULL);
    ASSERT_EQ(problem.object_count, 0u);
    ASSERT_EQ(problem.init_count, 0u);
    ASSERT_EQ(problem.goal_count, 0u);
}

UTEST(parse_problem, domain) {
    const char *str = "(define"
                      "    (:domain a)"
                      ")";

    Problem problem = {0};
    ProblemParse(&problem, str);

    ASSERT_EQ(problem.name.ptr, NULL);
    ASSERT_TRUE(strncmp(problem.domain.ptr, "a", problem.domain.len) == 0);
    ASSERT_EQ(problem.object_count, 0u);
    ASSERT_EQ(problem.init_count, 0u);
    ASSERT_EQ(problem.goal_count, 0u);
}

UTEST(parse_problem, objects) {
    const char *str = "(define"
                      "    (:objects a bb c-_)"
                      ")";

    Problem problem = {0};
    ProblemParse(&problem, str);

    ASSERT_EQ(problem.name.ptr, NULL);
    ASSERT_EQ(problem.domain.ptr, NULL);
    ASSERT_EQ(problem.object_count, 3u);
    ASSERT_EQ(problem.init_count, 0u);
    ASSERT_EQ(problem.goal_count, 0u);

    ASSERT_TRUE(strncmp(problem.objects[0].ptr, "a", problem.objects[0].len) == 0);
    ASSERT_TRUE(strncmp(problem.objects[1].ptr, "bb", problem.objects[1].len) == 0);
    ASSERT_TRUE(strncmp(problem.objects[2].ptr, "c-_", problem.objects[2].len) == 0);
}
