#include "pddl.h"

bool pddl_domain_validate(const pddl_domain* d) {
	return true;
}

bool pddl_problem_validate(const pddl_domain* d, const pddl_problem* p) {
	return true;
}

bool pddl_validate(const pddl_domain* d, const pddl_problem* p) {
        return pddl_domain_validate(d) && pddl_problem_validate(d, p);
}
