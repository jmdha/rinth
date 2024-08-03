#include <criterion/criterion.h>

#include "parse/domain.h"

Test(parse_domain, empty) {
    const char *str = "(define)";

    Domain domain = DomainParse(str);

    cr_assert_eq(domain.name, NULL);
    cr_assert_eq(domain.requirement_count, 0);
    cr_assert_eq(domain.predicate_count, 0);
    cr_assert_eq(domain.action_count, 0);

    DomainDelete(&domain);
}
