#include <criterion/criterion.h>

#include "parse/domain.h"

Test(parse_domain, empty) {
    const char *str = "(define)";

    Domain domain; 
    DomainParse(&domain, str);

    cr_assert_eq(domain.name.ptr, NULL);
    cr_assert_eq(domain.requirement_count, 0);
    cr_assert_eq(domain.predicate_count, 0);
    cr_assert_eq(domain.action_count, 0);
}

Test(parse_domain, name) {
    const char *str = "(define (domain abc))";

    Domain domain;
    DomainParse(&domain, str);

    cr_assert(StringCmp_s(&domain.name, "abc"));
}

Test(parse_domain, predicates) {
    const char *str = "(define (:predicates (a) (b ?x) (c ?x ?y)))";

    Domain domain;
    DomainParse(&domain, str);

    cr_assert_eq(domain.predicate_count, 3);
    cr_assert(StringCmp_s(&domain.predicates[0].name, "a"));
    cr_assert(StringCmp_s(&domain.predicates[1].name, "b"));
    cr_assert(StringCmp_s(&domain.predicates[2].name, "c"));
    cr_assert_eq(domain.predicates[0].var_count, 0);
    cr_assert_eq(domain.predicates[1].var_count, 1);
    cr_assert_eq(domain.predicates[2].var_count, 2);
    cr_assert(StringCmp_s(&domain.predicates[1].vars[0], "?x"));
    cr_assert(StringCmp_s(&domain.predicates[2].vars[0], "?x"));
    cr_assert(StringCmp_s(&domain.predicates[2].vars[1], "?y"));
}
