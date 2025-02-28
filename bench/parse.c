#include <ubench/ubench.h>

#include "parse.h"

static const char *DOMAIN = "(define"
                            "	(domain blocksworld)"
                            "	(:requirements :strips)"
                            "	(:predicates"
                            "		(clear ?x)"
                            "		(on-table ?x)"
                            "		(arm-empty)"
                            "		(holding ?x)"
                            "		(on ?x ?y)"
                            "	)"
                            "	(:action pickup"
                            "		:parameters (?ob)"
                            "		:precondition "
                            "			(and"
                            "				(clear ?ob)"
                            "				(on-table ?ob)"
                            "				(arm-empty)"
                            "			)"
                            "		:effect "
                            "			(and"
                            "				(holding ?ob)"
                            "				(not (clear ?ob))"
                            "				(not (on-table ?ob))"
                            "				(not (arm-empty))"
                            "			)"
                            "	)"
                            "	(:action putdown"
                            "		:parameters (?ob)"
                            "		:precondition "
                            "			(holding ?ob)"
                            "		:effect "
                            "			(and"
                            "				(clear ?ob)"
                            "				(arm-empty)"
                            "				(on-table ?ob)"
                            "				(not (holding ?ob))"
                            "			)"
                            "	)"
                            "	(:action stack"
                            "		:parameters (?ob ?underob)"
                            "		:precondition "
                            "			(and"
                            "				(clear ?underob)"
                            "				(holding ?ob)"
                            "			)"
                            "		:effect "
                            "			(and"
                            "				(arm-empty)"
                            "				(clear ?ob)"
                            "				(on ?ob ?underob)"
                            "				(not (clear ?underob))"
                            "				(not (holding ?ob))"
                            "			)"
                            "	)"
                            "	(:action unstack"
                            "		:parameters (?ob ?underob)"
                            "		:precondition "
                            "			(and"
                            "				(on ?ob ?underob)"
                            "				(clear ?ob)"
                            "				(arm-empty)"
                            "			)"
                            "		:effect "
                            "			(and"
                            "				(holding ?ob)"
                            "				(clear ?underob)"
                            "				(not (on ?ob ?underob))"
                            "				(not (clear ?ob))"
                            "				(not (arm-empty))"
                            "			)"
                            "	)"
                            ")";

static const char *PROBLEM = "(define (problem blocksworld-01)"
                             " (:domain blocksworld)"
                             " (:objects b1 b2)"
                             " (:init "
                             "    (arm-empty)"
                             "    (clear b2)"
                             "    (on-table b2)"
                             "    (clear b1)"
                             "    (on-table b1)"
                             " )"
                             " (:goal (and "
                             "    (clear b1)"
                             "    (on b1 b2)"
                             "    (on-table b2)"
                             ")))";

UBENCH_EX(parse, domain) {
    struct domain domain;
    printf("domain %lu\n", strlen(DOMAIN));
    UBENCH_DO_BENCHMARK() {
        for (uint i = 0; i < 100; i++)
            parse_domain_(&domain, DOMAIN);
    }
}

UBENCH_EX(parse, problem) {
    struct problem problem;
    printf("problem %lu\n", strlen(PROBLEM));
    UBENCH_DO_BENCHMARK() {
        for (uint i = 0; i < 1000; i++)
            parse_problem_(&problem, PROBLEM);
    }
}
