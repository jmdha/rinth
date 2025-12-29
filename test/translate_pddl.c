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

        ASSERT_EQ(NULL, def.predicates[0].ptr);
        ASSERT_EQ(NULL, def.actions[0].name.ptr);
        ASSERT_EQ(SIZE_MAX, def.inits[0].predicate);
        ASSERT_EQ(SIZE_MAX, def.goals[0].predicate);

        ASSERT_EQ(1, def.objects[0].len);
        ASSERT_EQ(2, def.objects[1].len);
        ASSERT_STRNEQ("a", def.objects[0].ptr, def.objects[0].len);
        ASSERT_STRNEQ("ab", def.objects[1].ptr, def.objects[1].len);
        ASSERT_EQ(NULL, def.objects[2].ptr);
}

UTEST(translate_pddl, action_predicates) {
        const char* domain_str =
            "(define (:predicates (a)) (:action abc :precondition (and (a)) :effect (and (a))))";
        const char*        problem_str = "(define)";
        const pddl_domain  domain      = pddl_domain_parse(domain_str);
        const pddl_problem problem     = pddl_problem_parse(problem_str);

        const task def = translate_pddl(&domain, &problem);

        ASSERT_EQ(3, def.actions[0].name.len);
        ASSERT_STRNEQ("abc", def.actions[0].name.ptr, def.actions[0].name.len);

        ASSERT_EQ(0, def.actions[0].arity);

        ASSERT_EQ(0, def.actions[0].pre_pos[0].predicate);
        ASSERT_EQ(SIZE_MAX, def.actions[0].pre_neg[0].predicate);
        ASSERT_EQ(0, def.actions[0].eff_pos[0].predicate);
        ASSERT_EQ(SIZE_MAX, def.actions[0].eff_neg[0].predicate);
}

UTEST(translate_pddl, action_vars) {
        const char* domain_str =
            "(define (:predicates (a ?a ?b)) (:action abc :parameters (v0 v1) :precondition (and (a v0 v1))))";
        const char*        problem_str = "(define)";
        const pddl_domain  domain      = pddl_domain_parse(domain_str);
        const pddl_problem problem     = pddl_problem_parse(problem_str);

        const task def = translate_pddl(&domain, &problem);

        ASSERT_EQ(3, def.actions[0].name.len);
        ASSERT_STRNEQ("abc", def.actions[0].name.ptr, def.actions[0].name.len);

        ASSERT_EQ(2, def.actions[0].arity);

        ASSERT_EQ(0, def.actions[0].pre_pos[0].predicate);
        ASSERT_EQ(SIZE_MAX, def.actions[0].pre_neg[0].predicate);
        ASSERT_EQ(SIZE_MAX, def.actions[0].eff_pos[0].predicate);
        ASSERT_EQ(SIZE_MAX, def.actions[0].eff_neg[0].predicate);

	ASSERT_EQ(2, def.actions[0].pre_pos[0].arity);
	ASSERT_EQ(0, def.actions[0].pre_pos[0].vars[0]);
	ASSERT_EQ(1, def.actions[0].pre_pos[0].vars[1]);
}

UTEST(translate_pddl, action_negative) {
        const char* domain_str =
            "(define (:predicates (a)) (:action abc :precondition (and (not (a)))))";
        const char*        problem_str = "(define)";
        const pddl_domain  domain      = pddl_domain_parse(domain_str);
        const pddl_problem problem     = pddl_problem_parse(problem_str);

        const task def = translate_pddl(&domain, &problem);

        ASSERT_EQ(3, def.actions[0].name.len);
        ASSERT_STRNEQ("abc", def.actions[0].name.ptr, def.actions[0].name.len);

        ASSERT_EQ(SIZE_MAX, def.actions[0].pre_pos[0].predicate);
        ASSERT_EQ(0, def.actions[0].pre_neg[0].predicate);
        ASSERT_EQ(SIZE_MAX, def.actions[0].eff_pos[0].predicate);
        ASSERT_EQ(SIZE_MAX, def.actions[0].eff_neg[0].predicate);
}

UTEST(translate_pddl, action_multi_negative) {
        const char* domain_str =
            "(define (:predicates (a)) (:action abc :precondition (and (not (not (a))))))";
        const char*        problem_str = "(define)";
        const pddl_domain  domain      = pddl_domain_parse(domain_str);
        const pddl_problem problem     = pddl_problem_parse(problem_str);

        const task def = translate_pddl(&domain, &problem);

        ASSERT_EQ(3, def.actions[0].name.len);
        ASSERT_STRNEQ("abc", def.actions[0].name.ptr, def.actions[0].name.len);

        ASSERT_EQ(0, def.actions[0].pre_pos[0].predicate);
        ASSERT_EQ(SIZE_MAX, def.actions[0].pre_neg[0].predicate);
        ASSERT_EQ(SIZE_MAX, def.actions[0].eff_pos[0].predicate);
        ASSERT_EQ(SIZE_MAX, def.actions[0].eff_neg[0].predicate);
}

UTEST(translate_pddl, inits_nullary) {
        const char* domain_str = "(define (:predicates (a)))";
        const char*        problem_str = "(define (:init (a)))";
        const pddl_domain  domain      = pddl_domain_parse(domain_str);
        const pddl_problem problem     = pddl_problem_parse(problem_str);

        const task def = translate_pddl(&domain, &problem);

	ASSERT_EQ(0, def.inits[0].predicate);
	ASSERT_EQ(SIZE_MAX, def.inits[1].predicate);
}

UTEST(translate_pddl, inits_unary) {
        const char* domain_str = "(define (:predicates (a ?a)))";
        const char*        problem_str = "(define (:objects o1) (:init (a o1)))";
        const pddl_domain  domain      = pddl_domain_parse(domain_str);
        const pddl_problem problem     = pddl_problem_parse(problem_str);

        const task def = translate_pddl(&domain, &problem);

	ASSERT_EQ(0, def.inits[0].predicate);
	ASSERT_EQ(SIZE_MAX, def.inits[1].predicate);

	ASSERT_EQ(1, def.inits[0].arity);
	ASSERT_EQ(0, def.inits[0].vars[0]);
}

UTEST(translate_pddl, inits_nary) {
        const char* domain_str = "(define (:predicates (a ?a ?b)))";
        const char*        problem_str = "(define (:objects o1 o2) (:init (a o2 o1)))";
        const pddl_domain  domain      = pddl_domain_parse(domain_str);
        const pddl_problem problem     = pddl_problem_parse(problem_str);

        const task def = translate_pddl(&domain, &problem);

	ASSERT_EQ(0, def.inits[0].predicate);
	ASSERT_EQ(SIZE_MAX, def.inits[1].predicate);

	ASSERT_EQ(2, def.inits[0].arity);
	ASSERT_EQ(1, def.inits[0].vars[0]);
	ASSERT_EQ(0, def.inits[0].vars[1]);
}

UTEST(translate_pddl, inits_multiple) {
        const char* domain_str = "(define (:predicates (a) (b)))";
        const char*        problem_str = "(define (:init (a) (b)))";
        const pddl_domain  domain      = pddl_domain_parse(domain_str);
        const pddl_problem problem     = pddl_problem_parse(problem_str);

        const task def = translate_pddl(&domain, &problem);

	ASSERT_EQ(0, def.inits[0].predicate);
	ASSERT_EQ(1, def.inits[1].predicate);
	ASSERT_EQ(SIZE_MAX, def.inits[2].predicate);
}
