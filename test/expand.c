#include <utest.h/utest.h>

#include "pddl.h"
#include "translate.h"
#include "expand.h"

const char* DOMAIN = "(define"
                     "(:predicates (conn ?a ?b) (on ?a))"
                     "(:action move"
                     "     :parameters (?a ?b)"
                     "     :precondition (and"
                     "          (conn ?a ?b)"
                     "          (on ?a)"
                     "      )"
                     "      :effect (and"
                     "          (not (on ?a))"
                     "          (on ?b)"
                     "      )"
                     ")"
                     ")";

const char* PROBLEM = "(define"
                      "(:objects c0 c1)"
                      "(:init"
                      "    (conn c0 c1)"
                      "    (conn c1 c0)"
                      "    (on c0)"
                      ")"
                      "(:goal (and"
                      "    (on c1)"
                      "))"
                      ")";

UTEST(expand_cp, 1_step) {
        const pddl_domain  domain      = pddl_domain_parse(DOMAIN);
        const pddl_problem problem     = pddl_problem_parse(PROBLEM);
        task def                       = translate_pddl(&domain, &problem);

	expand_init_cp(&def);
	expand_cp(def.init);
	
	size_t action;
	size_t args[64];

	ASSERT_TRUE(expand_step_cp(&action, args));
	ASSERT_EQ(0, action);
	ASSERT_EQ(0, args[0]);
	ASSERT_EQ(1, args[1]);

	task_free(&def);
}
