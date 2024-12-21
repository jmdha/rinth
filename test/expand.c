#include <utest.h/utest.h>

#include "parse.h"
#include "task.h"
#include "expand.h"

const char* CELLS_DOMAIN = 
    "(define"
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

const char* CELLS_PROBLEM_0 = 
    "(define"
    "(:objects c0 c1)"
    "(:init"
    "    (conn c0 c1)"
    "    (on c0)"
    ")"
    "(:goal (and"
    "    (on c1)"
    "))"
    ")";

UTEST(expand, cells_0) {
    struct domain domain   = parse_domain(CELLS_DOMAIN);
    struct problem problem = parse_problem(CELLS_PROBLEM_0);
    struct task task       = translate(&domain, &problem);
    expand_init(&task);
    domain_free(&domain);
    task_free(&task);

    ASSERT_EQ(expand_count(task.init), 1u);
}
