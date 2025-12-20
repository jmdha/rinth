#include <utest.h/utest.h>

#include "parse.h"
#include "translate.h"
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

const char* CELLS_PROBLEM_1 = 
    "(define"
    "(:objects c0 c1 c2 c3 c4)"
    "(:init"
    "    (conn c0 c1)"
    "    (conn c0 c2)"
    "    (conn c0 c3)"
    "    (conn c0 c4)"
    "    (on c0)"
    ")"
    "(:goal (and"
    "    (on c1)"
    "))"
    ")";

UTEST(expand, cells_0) {
    struct domain domain   = parse_domain(CELLS_DOMAIN);
    struct problem problem = parse_problem(CELLS_PROBLEM_0);
    task_t task       = translate(&domain, &problem);
    expand_init(&task);

    expand(task.init);
    ASSERT_EQ(expand_count(task.init), 1u);
    domain_free(&domain);
    task_free(&task);
}

UTEST(expand, cells_1) {
    struct domain domain   = parse_domain(CELLS_DOMAIN);
    struct problem problem = parse_problem(CELLS_PROBLEM_1);
    task_t task       = translate(&domain, &problem);
    expand_init(&task);

    expand(task.init);
    ASSERT_EQ(expand_count(task.init), 4u);
    domain_free(&domain);
    task_free(&task);
}
