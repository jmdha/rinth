#include <utest.h/utest.h>

#include "parse.h"

UTEST(parse, domain_empty) {
    const char *str = "(define)";

    struct domain domain = parse_domain(str);

    ASSERT_EQ(domain.requirement_count, 0u);
    ASSERT_EQ(domain.predicate_count, 0u);
    ASSERT_EQ(domain.action_count, 0u);
    domain_free(&domain);
}

UTEST(parse, domain_name) {
    const char *str = "(define (domain abc))";

    struct domain domain = parse_domain(str);

    ASSERT_TRUE(str_cmp_s(&domain.name, "abc"));
    domain_free(&domain);
}

UTEST(parse, domain_types_untyped) {
    const char *str = "(define)";

    struct domain domain = parse_domain(str);

    ASSERT_EQ(domain.type_count, 0u);
    domain_free(&domain);
}

UTEST(parse, domain_types_empty) {
    const char *str = "(define (:types))";

    struct domain domain = parse_domain(str);

    ASSERT_EQ(domain.type_count, 0u);
    domain_free(&domain);
}

UTEST(parse, domain_types_single) {
    const char *str = "(define (:types object))";

    struct domain domain = parse_domain(str);

    ASSERT_EQ(domain.type_count, 1u);
    ASSERT_TRUE(str_cmp_s(&domain.types[0], "object"));
    ASSERT_EQ(domain.type_parents[0].len, 0u);
    ASSERT_EQ(domain.type_parents[0].ptr, NULL);
    domain_free(&domain);
}

UTEST(parse, domain_types_parent) {
    const char *str = "(define (:types a - b))";

    struct domain domain = parse_domain(str);

    ASSERT_EQ(domain.type_count, 2u);
    ASSERT_TRUE(str_cmp_s(&domain.types[0], "a"));
    ASSERT_TRUE(str_cmp_s(&domain.types[1], "b"));
    ASSERT_TRUE(str_cmp_s(&domain.type_parents[0], "b"));
    ASSERT_EQ(domain.type_parents[1].len, 0u);
    ASSERT_EQ(domain.type_parents[1].ptr, NULL);
    domain_free(&domain);
}

UTEST(parse, domain_types_multichild) {
    const char *str = "(define (:types a b - c))";

    struct domain domain = parse_domain(str);

    ASSERT_EQ(domain.type_count, 3u);
    ASSERT_TRUE(str_cmp_s(&domain.types[0], "a"));
    ASSERT_TRUE(str_cmp_s(&domain.types[1], "b"));
    ASSERT_TRUE(str_cmp_s(&domain.types[2], "c"));
    ASSERT_TRUE(str_cmp_s(&domain.type_parents[0], "c"));
    ASSERT_TRUE(str_cmp_s(&domain.type_parents[1], "c"));
    ASSERT_EQ(domain.type_parents[2].len, 0u);
    ASSERT_EQ(domain.type_parents[2].ptr, NULL);
    domain_free(&domain);
}

UTEST(parse, domain_types_multiparent) {
    const char *str = "(define (:types a - b c - d))";

    struct domain domain = parse_domain(str);

    ASSERT_EQ(domain.type_count, 4u);
    ASSERT_TRUE(str_cmp_s(&domain.types[0], "a"));
    ASSERT_TRUE(str_cmp_s(&domain.types[1], "b"));
    ASSERT_TRUE(str_cmp_s(&domain.types[2], "c"));
    ASSERT_TRUE(str_cmp_s(&domain.types[3], "d"));
    ASSERT_TRUE(str_cmp_s(&domain.type_parents[0], "b"));
    ASSERT_TRUE(str_cmp_s(&domain.type_parents[2], "d"));
    ASSERT_EQ(domain.type_parents[3].len, 0u);
    ASSERT_EQ(domain.type_parents[3].ptr, NULL);
    domain_free(&domain);
}

UTEST(parse, domain_types_orphan) {
    const char *str = "(define (:types a - b c))";

    struct domain domain = parse_domain(str);

    ASSERT_EQ(domain.type_count, 3u);
    ASSERT_TRUE(str_cmp_s(&domain.types[0], "a"));
    ASSERT_TRUE(str_cmp_s(&domain.types[1], "b"));
    ASSERT_TRUE(str_cmp_s(&domain.types[2], "c"));
    ASSERT_TRUE(str_cmp_s(&domain.type_parents[0], "b"));
    ASSERT_EQ(domain.type_parents[2].len, 0u);
    ASSERT_EQ(domain.type_parents[2].ptr, NULL);
    domain_free(&domain);
}

UTEST(parse, domain_types_grandparents) {
    const char *str = "(define (:types a - b b - c))";

    struct domain domain = parse_domain(str);

    ASSERT_EQ(domain.type_count, 3u);
    ASSERT_TRUE(str_cmp_s(&domain.types[0], "a"));
    ASSERT_TRUE(str_cmp_s(&domain.types[1], "b"));
    ASSERT_TRUE(str_cmp_s(&domain.types[2], "c"));
    ASSERT_TRUE(str_cmp_s(&domain.type_parents[0], "b"));
    ASSERT_TRUE(str_cmp_s(&domain.type_parents[1], "c"));
    ASSERT_EQ(domain.type_parents[2].len, 0u);
    ASSERT_EQ(domain.type_parents[2].ptr, NULL);
    domain_free(&domain);
}

UTEST(parse, domain_types_grandchildren) {
    const char *str = "(define (:types a - b b c - d))";

    struct domain domain = parse_domain(str);

    ASSERT_EQ(domain.type_count, 4u);
    ASSERT_TRUE(str_cmp_s(&domain.types[0], "a"));
    ASSERT_TRUE(str_cmp_s(&domain.types[1], "b"));
    ASSERT_TRUE(str_cmp_s(&domain.types[2], "c"));
    ASSERT_TRUE(str_cmp_s(&domain.types[3], "d"));
    ASSERT_TRUE(str_cmp_s(&domain.type_parents[0], "b"));
    ASSERT_TRUE(str_cmp_s(&domain.type_parents[1], "d"));
    ASSERT_TRUE(str_cmp_s(&domain.type_parents[2], "d"));
    ASSERT_EQ(domain.type_parents[3].len, 0u);
    ASSERT_EQ(domain.type_parents[3].ptr, NULL);
    domain_free(&domain);
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
    domain_free(&domain);
}

UTEST(parse, domain_predicates_typed) {
    const char *str = "(define (:predicates (a ?a - t1) (b ?a ?b - t2 ?c)))";

    struct domain domain = parse_domain(str);

    ASSERT_EQ(domain.predicate_count, 2u);
    ASSERT_TRUE(str_cmp_s(&domain.predicates[0].name, "a"));
    ASSERT_EQ(domain.predicates[0].var_count, 1u);
    ASSERT_TRUE(str_cmp_s(&domain.predicates[0].vars[0], "?a"));
    ASSERT_TRUE(str_cmp_s(&domain.predicates[0].var_types[0], "t1"));
    ASSERT_TRUE(str_cmp_s(&domain.predicates[1].name, "b"));
    ASSERT_EQ(domain.predicates[1].var_count, 3u);
    ASSERT_TRUE(str_cmp_s(&domain.predicates[1].vars[0], "?a"));
    ASSERT_TRUE(str_cmp_s(&domain.predicates[1].vars[1], "?b"));
    ASSERT_TRUE(str_cmp_s(&domain.predicates[1].vars[2], "?c"));
    ASSERT_TRUE(str_cmp_s(&domain.predicates[1].var_types[0], "t2"));
    ASSERT_TRUE(str_cmp_s(&domain.predicates[1].var_types[1], "t2"));
    ASSERT_TRUE(!domain.predicates[1].var_types[2].ptr);
    domain_free(&domain);
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

    ASSERT_TRUE(str_cmp_s(&problem.domain, "a"));
    ASSERT_EQ(problem.object_count, 0u);
}

UTEST(parse, problem_objects) {
    const char *str = "(define"
                      "    (:objects a bb c-_)"
                      ")";

    struct problem problem = parse_problem(str);

    ASSERT_EQ(problem.object_count, 3u);

    ASSERT_TRUE(str_cmp_s(&problem.objects[0], "a"));
    ASSERT_TRUE(str_cmp_s(&problem.objects[1], "bb"));
    ASSERT_TRUE(str_cmp_s(&problem.objects[2], "c-_"));

    ASSERT_EQ(problem.object_types[0].ptr, NULL);
    ASSERT_EQ(problem.object_types[1].ptr, NULL);
    ASSERT_EQ(problem.object_types[2].ptr, NULL);
}

UTEST(parse, problem_objects_typed) {
    const char *str = "(define"
                      "    (:objects o1 - t1 o2 o3 - t2 o4)"
                      ")";

    struct problem problem = parse_problem(str);

    ASSERT_EQ(problem.object_count, 4u);

    ASSERT_TRUE(str_cmp_s(&problem.objects[0], "o1"));
    ASSERT_TRUE(str_cmp_s(&problem.objects[1], "o2"));
    ASSERT_TRUE(str_cmp_s(&problem.objects[2], "o3"));
    ASSERT_TRUE(str_cmp_s(&problem.objects[3], "o4"));

    ASSERT_TRUE(str_cmp_s(&problem.object_types[0], "t1"));
    ASSERT_TRUE(str_cmp_s(&problem.object_types[1], "t2"));
    ASSERT_TRUE(str_cmp_s(&problem.object_types[2], "t2"));
    ASSERT_EQ(problem.object_types[3].ptr, NULL);
}
