#include <utest.h/utest.h>

#include "misc.h"
#include "parse.h"
#include "translate.h"

UTEST(translate, name) {
    const char* domain  = "(define (domain  lorem))";
    const char* problem = "(define (problem ipsum))";

    struct domain  d = parse_domain(domain);
    struct problem p = parse_problem(problem);
    struct task    t = translate(&d, &p);

    ASSERT_TRUE(str_cmp_s(&t.domain_name,  "lorem"));
    ASSERT_TRUE(str_cmp_s(&t.problem_name, "ipsum"));

    domain_free(&d);
    task_free(&t);
}

UTEST(translate, predicates) {
    const char* domain  = "(define (:predicates (p0) (p1 ?a) (p2 ?a ?b)))";
    const char* problem = "(define)";

    struct domain  d = parse_domain(domain);
    struct problem p = parse_problem(problem);
    struct task    t = translate(&d, &p);

    ASSERT_EQ(t.predicate_count, 3u);

    ASSERT_EQ(t.predicate_vars[0], 0u);
    ASSERT_EQ(t.predicate_vars[1], 1u);
    ASSERT_EQ(t.predicate_vars[2], 2u);

    ASSERT_TRUE(str_cmp_s(&t.predicates[0], "p0"));
    ASSERT_TRUE(str_cmp_s(&t.predicates[1], "p1"));
    ASSERT_TRUE(str_cmp_s(&t.predicates[2], "p2"));

    domain_free(&d);
    task_free(&t);
}

UTEST(translate, action_trivial) {
    const char* domain  = 
    "(define                       \
	    (:predicates (p0))     \
            (:action a0            \
	        :effect (p0)       \
	    )                      \
    )";
    const char* problem = "(define)";

    struct domain  d = parse_domain(domain);
    struct problem p = parse_problem(problem);
    struct task    t = translate(&d, &p);

    ASSERT_EQ(t.scheme_count, 1u);

    ASSERT_TRUE(str_cmp_s(&t.schemes[0].name, "a0"));
    ASSERT_EQ(t.schemes[0].vars, 0u);
    ASSERT_EQ(t.schemes[0].pre_count, 0u);
    ASSERT_EQ(t.schemes[0].eff_count, 1u);
    ASSERT_EQ(t.schemes[0].eff[0].predicate, 0u);
    ASSERT_EQ(t.schemes[0].eff[0].arg_count, 0u);
    ASSERT_EQ(t.schemes[0].eff[0].val, true);

    domain_free(&d);
    task_free(&t);
}

UTEST(translate, action_negation) {
    const char* domain  = 
    "(define                       \
	    (:predicates (p0))     \
            (:action a0            \
	        :effect (not (p0)) \
	    )                      \
    )";
    const char* problem = "(define)";

    struct domain  d = parse_domain(domain);
    struct problem p = parse_problem(problem);
    struct task    t = translate(&d, &p);

    ASSERT_EQ(t.scheme_count, 1u);

    ASSERT_TRUE(str_cmp_s(&t.schemes[0].name, "a0"));
    ASSERT_EQ(t.schemes[0].vars, 0u);
    ASSERT_EQ(t.schemes[0].pre_count, 0u);
    ASSERT_EQ(t.schemes[0].eff_count, 1u);
    ASSERT_EQ(t.schemes[0].eff[0].predicate, 0u);
    ASSERT_EQ(t.schemes[0].eff[0].arg_count, 0u);
    ASSERT_EQ(t.schemes[0].eff[0].val, false);

    domain_free(&d);
    task_free(&t);
}

UTEST(translate, action_and) {
    const char* domain  = 
    "(define                            \
	    (:predicates (p0) (p1))     \
            (:action a0                 \
	        :effect (and (p0) (p1)) \
	    )                           \
    )";
    const char* problem = "(define)";

    struct domain  d = parse_domain(domain);
    struct problem p = parse_problem(problem);
    struct task    t = translate(&d, &p);

    ASSERT_EQ(t.scheme_count, 1u);

    ASSERT_TRUE(str_cmp_s(&t.schemes[0].name, "a0"));
    ASSERT_EQ(t.schemes[0].vars, 0u);
    ASSERT_EQ(t.schemes[0].pre_count, 0u);
    ASSERT_EQ(t.schemes[0].eff_count, 2u);
    ASSERT_EQ(t.schemes[0].eff[0].predicate, 0u);
    ASSERT_EQ(t.schemes[0].eff[0].arg_count, 0u);
    ASSERT_EQ(t.schemes[0].eff[0].val, false);
    ASSERT_EQ(t.schemes[0].eff[1].predicate, 1u);
    ASSERT_EQ(t.schemes[0].eff[1].arg_count, 0u);
    ASSERT_EQ(t.schemes[0].eff[1].val, false);

    domain_free(&d);
    task_free(&t);
}

UTEST(translate, action_or) {
    const char* domain  = 
    "(define                           \
	    (:predicates (p0) (p1))    \
            (:action a0                \
	        :effect (or (p0) (p1)) \
	    )                          \
    )";
    const char* problem = "(define)";

    struct domain  d = parse_domain(domain);
    struct problem p = parse_problem(problem);
    struct task    t = translate(&d, &p);

    ASSERT_EQ(t.scheme_count, 2u);

    ASSERT_TRUE(str_cmp_s(&t.schemes[0].name, "a0"));
    ASSERT_EQ(t.schemes[0].vars, 0u);
    ASSERT_EQ(t.schemes[0].pre_count, 0u);
    ASSERT_EQ(t.schemes[0].eff_count, 2u);
    ASSERT_EQ(t.schemes[0].eff[0].predicate, 0u);
    ASSERT_EQ(t.schemes[0].eff[0].arg_count, 0u);
    ASSERT_EQ(t.schemes[0].eff[0].val, false);

    ASSERT_TRUE(str_cmp_s(&t.schemes[1].name, "a0"));
    ASSERT_EQ(t.schemes[1].vars, 0u);
    ASSERT_EQ(t.schemes[1].pre_count, 0u);
    ASSERT_EQ(t.schemes[1].eff_count, 2u);
    ASSERT_EQ(t.schemes[1].eff[0].predicate, 1u);
    ASSERT_EQ(t.schemes[1].eff[0].arg_count, 0u);
    ASSERT_EQ(t.schemes[1].eff[0].val, false);

    domain_free(&d);
    task_free(&t);
}
