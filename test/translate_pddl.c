#include <utest.h/utest.h>

#include "misc.h"
#include "translate.h"

UTEST(translate_pddl, empty) {
        const char*        domain_str  = "(define)";
        const char*        problem_str = "(define)";
        const pddl_domain  domain      = pddl_domain_parse(domain_str);
        const pddl_problem problem     = pddl_problem_parse(problem_str);

        const task def = translate_pddl(&domain, &problem);

        ASSERT_EQ(NULL, def.predicates[0].ptr);
        ASSERT_EQ(NULL, def.objects[0].ptr);
        ASSERT_EQ(NULL, def.actions[0].name.ptr);
        ASSERT_EQ(SIZE_MAX, def.inits[0].predicate);
        ASSERT_EQ(SIZE_MAX, def.goals[0].predicate);
}

UTEST(translate_pddl, predicates) {
        const char*        domain_str  = "(define (:predicates (a) (b ?a)))";
        const char*        problem_str = "(define)";
        const pddl_domain  domain      = pddl_domain_parse(domain_str);
        const pddl_problem problem     = pddl_problem_parse(problem_str);

        const task def = translate_pddl(&domain, &problem);

        ASSERT_EQ(NULL, def.objects[0].ptr);
        ASSERT_EQ(NULL, def.actions[0].name.ptr);
        ASSERT_EQ(SIZE_MAX, def.inits[0].predicate);
        ASSERT_EQ(SIZE_MAX, def.goals[0].predicate);

        ASSERT_EQ(1, def.predicates[0].len);
        ASSERT_EQ(1, def.predicates[1].len);
        ASSERT_STRNEQ("a", def.predicates[0].ptr, def.predicates[0].len);
        ASSERT_STRNEQ("b", def.predicates[1].ptr, def.predicates[1].len);
}

UTEST(translate_pddl, objects) {
        const char*        domain_str  = "(define)";
        const char*        problem_str = "(define (:objects a ab))";
        const pddl_domain  domain      = pddl_domain_parse(domain_str);
        const pddl_problem problem     = pddl_problem_parse(problem_str);

        const task def = translate_pddl(&domain, &problem);

        ASSERT_EQ(NULL, def.objects[0].ptr);
        ASSERT_EQ(NULL, def.actions[0].name.ptr);
        ASSERT_EQ(SIZE_MAX, def.inits[0].predicate);
        ASSERT_EQ(SIZE_MAX, def.goals[0].predicate);

        ASSERT_EQ(1, def.objects[0].len);
        ASSERT_EQ(2, def.objects[1].len);
        ASSERT_STRNEQ("a", def.objects[0].ptr, def.objects[0].len);
        ASSERT_STRNEQ("ab", def.objects[1].ptr, def.objects[1].len);
}
