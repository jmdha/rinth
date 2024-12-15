#include <utest.h/utest.h>

#include "parse.h"

UTEST(parse, domain_empty) {
    const char *str = "(define)";

    struct domain domain = parse_domain(str);

    ASSERT_EQ(domain.requirement_count, 0u);
    ASSERT_EQ(domain.predicate_count, 0u);
    ASSERT_EQ(domain.action_count, 0u);
}

UTEST(parse, domain_name) {
    const char *str = "(define (domain abc))";

    struct domain domain = parse_domain(str);

    ASSERT_TRUE(str_cmp_s(&domain.name, "abc"));
}

UTEST(parse, domain_predicates) {
    const char *str = "(define (:predicates (a) (b ?x) (c ?x ?y)))";

    struct domain domain = parse_domain(str);

    ASSERT_EQ(domain.predicate_count, 3u);
    ASSERT_TRUE(str_cmp_s(&domain.predicates[0].name, "a"));
    ASSERT_TRUE(str_cmp_s(&domain.predicates[1].name, "b"));
    ASSERT_TRUE(str_cmp_s(&domain.predicates[2].name, "c"));
    ASSERT_EQ(domain.predicates[0].var_count, 0u);
    ASSERT_EQ(domain.predicates[1].var_count, 1u);
    ASSERT_EQ(domain.predicates[2].var_count, 2u);
    ASSERT_TRUE(str_cmp_s(&domain.predicates[1].vars[0], "?x"));
    ASSERT_TRUE(str_cmp_s(&domain.predicates[2].vars[0], "?x"));
    ASSERT_TRUE(str_cmp_s(&domain.predicates[2].vars[1], "?y"));
}

UTEST(parse, problem_empty) {
    const char *str = "(define"
                      ")";

    struct problem problem = parse_problem(str);

    ASSERT_EQ(problem.name.ptr, NULL);
    ASSERT_EQ(problem.domain.ptr, NULL);
    ASSERT_EQ(problem.object_count, 0u);
    ASSERT_EQ(problem.init_count, 0u);
    ASSERT_EQ(problem.goal_count, 0u);
}

UTEST(parse, problem_name) {
    const char *str = "(define"
                      "    (problem a)"
                      ")";

    struct problem problem = parse_problem(str);

    ASSERT_TRUE(strncmp(problem.name.ptr, "a", problem.name.len) == 0);
    ASSERT_EQ(problem.domain.ptr, NULL);
    ASSERT_EQ(problem.object_count, 0u);
    ASSERT_EQ(problem.init_count, 0u);
    ASSERT_EQ(problem.goal_count, 0u);
}

UTEST(parse, problem_domain) {
    const char *str = "(define"
                      "    (:domain a)"
                      ")";

    struct problem problem = parse_problem(str);

    ASSERT_EQ(problem.name.ptr, NULL);
    ASSERT_TRUE(strncmp(problem.domain.ptr, "a", problem.domain.len) == 0);
    ASSERT_EQ(problem.object_count, 0u);
    ASSERT_EQ(problem.init_count, 0u);
    ASSERT_EQ(problem.goal_count, 0u);
}

UTEST(parse, problem_objects) {
    const char *str = "(define"
                      "    (:objects a bb c-_)"
                      ")";

    struct problem problem = parse_problem(str);

    ASSERT_EQ(problem.name.ptr, NULL);
    ASSERT_EQ(problem.domain.ptr, NULL);
    ASSERT_EQ(problem.object_count, 3u);
    ASSERT_EQ(problem.init_count, 0u);
    ASSERT_EQ(problem.goal_count, 0u);

    ASSERT_TRUE(strncmp(problem.objects[0].ptr, "a", problem.objects[0].len) == 0);
    ASSERT_TRUE(strncmp(problem.objects[1].ptr, "bb", problem.objects[1].len) == 0);
    ASSERT_TRUE(strncmp(problem.objects[2].ptr, "c-_", problem.objects[2].len) == 0);
}
