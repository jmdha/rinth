#include <utest.h/utest.h>

#include "pddl.h"

UTEST(pddl_parse_domain, empty) {
	const char* s = "(define)";

	pddl_domain d = pddl_domain_parse(s);

	ASSERT_EQ(NULL, d.name.ptr);
	ASSERT_EQ(NULL, d.requirements[0].ptr);
	ASSERT_EQ(NULL, d.predicates[0].name.ptr);
	ASSERT_EQ(NULL, d.actions[0].name.ptr);
}

UTEST(pddl_parse_domain, name) {
	const char* s = "(define (domain abc))";

	pddl_domain d = pddl_domain_parse(s);

	ASSERT_EQ(3, d.name.len);
	ASSERT_STRNEQ("abc", d.name.ptr, d.name.len);
}

UTEST(pddl_parse_domain, requirements) {
	const char* s = "(define (:requirements a b c))";

	pddl_domain d = pddl_domain_parse(s);

	ASSERT_EQ(1, d.requirements[0].len);
	ASSERT_EQ(1, d.requirements[1].len);
	ASSERT_EQ(1, d.requirements[2].len);
	ASSERT_STRNEQ("a", d.requirements[0].ptr, d.requirements[0].len);
	ASSERT_STRNEQ("b", d.requirements[1].ptr, d.requirements[1].len);
	ASSERT_STRNEQ("c", d.requirements[2].ptr, d.requirements[2].len);
	ASSERT_EQ(NULL, d.requirements[3].ptr);
	ASSERT_EQ(0, d.requirements[3].len);
}

UTEST(pddl_parse_domain, predicates) {
	const char* s = "(define (:predicates (a) (b ?x) (c ?x ?y)))";

	pddl_domain d = pddl_domain_parse(s);

	ASSERT_EQ(1, d.predicates[0].name.len);
	ASSERT_EQ(1, d.predicates[1].name.len);
	ASSERT_EQ(1, d.predicates[2].name.len);
	ASSERT_STRNEQ("a", d.predicates[0].name.ptr, d.predicates[0].name.len);
	ASSERT_STRNEQ("b", d.predicates[1].name.ptr, d.predicates[1].name.len);
	ASSERT_STRNEQ("c", d.predicates[2].name.ptr, d.predicates[2].name.len);
	ASSERT_EQ(NULL, d.predicates[3].name.ptr);
	ASSERT_EQ(0, d.predicates[3].name.len);
}

UTEST(pddl_parse_domain, action_name) {
	const char* s = "(define (:action abc))";

	pddl_domain d = pddl_domain_parse(s);

	ASSERT_EQ(3, d.actions[0].name.len);
	ASSERT_STRNEQ("abc", d.actions[0].name.ptr, d.actions[0].name.len);
}

UTEST(pddl_parse_domain, action_vars) {
	const char* s = "(define (:action a :parameters (a b)))";

	pddl_domain d = pddl_domain_parse(s);

	ASSERT_EQ(1, d.actions[0].name.len);
	ASSERT_STRNEQ("a", d.actions[0].name.ptr, d.actions[0].name.len);
	ASSERT_EQ(1, d.actions[0].vars[0].len);
	ASSERT_EQ(1, d.actions[0].vars[1].len);
	ASSERT_STRNEQ("a", d.actions[0].vars[0].ptr, d.actions[0].vars[0].len);
	ASSERT_STRNEQ("b", d.actions[0].vars[1].ptr, d.actions[0].vars[1].len);
	ASSERT_EQ(0, d.actions[0].vars[2].len);
	ASSERT_EQ(NULL, d.actions[0].vars[2].ptr);
}

UTEST(pddl_parse_domain, action_pre) {
	const char* s = "(define (:action a :precondition(and (p0))))";

	pddl_domain d = pddl_domain_parse(s);

	ASSERT_EQ(1, d.actions[0].name.len);
	ASSERT_STRNEQ("a", d.actions[0].name.ptr, d.actions[0].name.len);
	ASSERT_EQ(1, d.actions[0].precondition[0].len);
	ASSERT_EQ(3, d.actions[0].precondition[1].len);
	ASSERT_EQ(1, d.actions[0].precondition[2].len);
	ASSERT_EQ(2, d.actions[0].precondition[3].len);
	ASSERT_EQ(1, d.actions[0].precondition[4].len);
	ASSERT_EQ(1, d.actions[0].precondition[5].len);
	ASSERT_STRNEQ("(",   d.actions[0].precondition[0].ptr, d.actions[0].precondition[0].len);
	ASSERT_STRNEQ("and", d.actions[0].precondition[1].ptr, d.actions[0].precondition[1].len);
	ASSERT_STRNEQ("(",   d.actions[0].precondition[2].ptr, d.actions[0].precondition[2].len);
	ASSERT_STRNEQ("p0",  d.actions[0].precondition[3].ptr, d.actions[0].precondition[3].len);
	ASSERT_STRNEQ(")",   d.actions[0].precondition[4].ptr, d.actions[0].precondition[4].len);
	ASSERT_STRNEQ(")",   d.actions[0].precondition[5].ptr, d.actions[0].precondition[5].len);
	ASSERT_EQ(0, d.actions[0].precondition[6].len);
	ASSERT_EQ(NULL, d.actions[0].precondition[6].ptr);
}

UTEST(pddl_parse_problem, empty) {
	const char* s = "(define)";

	pddl_problem p = pddl_problem_parse(s);

	ASSERT_EQ(NULL, p.name.ptr);
	ASSERT_EQ(NULL, p.domain.ptr);
	ASSERT_EQ(NULL, p.objects[0].ptr);
	ASSERT_EQ(NULL, p.inits[0].predicate.ptr);
	ASSERT_EQ(NULL, p.goals[0].predicate.ptr);
}

UTEST(pddl_parse_problem, name) {
	const char* s = "(define (problem abc))";

	pddl_problem p = pddl_problem_parse(s);

	ASSERT_EQ(3, p.name.len);
	ASSERT_STRNEQ("abc", p.name.ptr, p.name.len);
}

UTEST(pddl_parse_problem, domain) {
	const char* s = "(define (:domain abc))";

	pddl_problem p = pddl_problem_parse(s);

	ASSERT_EQ(3, p.domain.len);
	ASSERT_STRNEQ("abc", p.domain.ptr, p.domain.len);
}

UTEST(pddl_parse_problem, objects) {
	const char* s = "(define (:objects a _b c-))";

	pddl_problem p = pddl_problem_parse(s);

	ASSERT_EQ(1, p.objects[0].len);
	ASSERT_EQ(2, p.objects[1].len);
	ASSERT_EQ(2, p.objects[2].len);
	ASSERT_STRNEQ("a",  p.objects[0].ptr, p.objects[0].len);
	ASSERT_STRNEQ("_b", p.objects[1].ptr, p.objects[1].len);
	ASSERT_STRNEQ("c-", p.objects[2].ptr, p.objects[2].len);
}

UTEST(pddl_parse_problem, inits) {
	const char* s = "(define (:init (a) (b c)))";

	pddl_problem p = pddl_problem_parse(s);

	ASSERT_EQ(1, p.inits[0].predicate.len);
	ASSERT_EQ(1, p.inits[1].predicate.len);
	ASSERT_STRNEQ("a",  p.inits[0].predicate.ptr, p.inits[0].predicate.len);
	ASSERT_STRNEQ("b",  p.inits[1].predicate.ptr, p.inits[1].predicate.len);
	ASSERT_EQ(NULL, p.inits[0].vars[0].ptr);
	ASSERT_EQ(NULL, p.inits[0].vars[2].ptr);
	ASSERT_EQ(1, p.inits[1].vars[0].len);
	ASSERT_STRNEQ("c", p.inits[1].vars[0].ptr, p.inits[1].vars[0].len);
}

UTEST(pddl_parse_problem, goals) {
	const char* s = "(define (:goal (and (a) (b c))))";

	pddl_problem p = pddl_problem_parse(s);

	ASSERT_EQ(1, p.goals[0].predicate.len);
	ASSERT_EQ(1, p.goals[1].predicate.len);
	ASSERT_STRNEQ("a",  p.goals[0].predicate.ptr, p.goals[0].predicate.len);
	ASSERT_STRNEQ("b",  p.goals[1].predicate.ptr, p.goals[1].predicate.len);
	ASSERT_EQ(NULL, p.goals[0].vars[0].ptr);
	ASSERT_EQ(NULL, p.goals[0].vars[2].ptr);
	ASSERT_EQ(1, p.goals[1].vars[0].len);
	ASSERT_STRNEQ("c", p.goals[1].vars[0].ptr, p.goals[1].vars[0].len);
}
