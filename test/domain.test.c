#include <utest.h/utest.h>

#include "domain.h"

UTEST(parse_domain, empty) {
    const char *str = "(define)";

    Domain domain = domain_parse(str); 

    ASSERT_EQ(domain.requirement_count, 0u);
    ASSERT_EQ(domain.predicate_count, 0u);
    ASSERT_EQ(domain.action_count, 0u);
}

UTEST(parse_domain, name) {
    const char *str = "(define (domain abc))";

    Domain domain = domain_parse(str);

    ASSERT_TRUE(str_cmp_s(&domain.name, "abc"));
}

UTEST(parse_domain, predicates) {
    const char *str = "(define (:predicates (a) (b ?x) (c ?x ?y)))";

    Domain domain = domain_parse(str);

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
