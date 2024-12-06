#include <limits.h>
#include <stdlib.h>

#include "log.h"
#include "verify.h"

bool VerifyFact(const Domain *domain, const Problem *problem, const SFact *fact) {
    const uint predicate_index = DomainPredicateIndex(domain, &fact->predicate);
    if (predicate_index == UINT_MAX) {
        char *predicate_name = StringAlloc(&fact->predicate);
        ERROR("Problem contains fact that referes to undefined predicate %s", predicate_name);
        free(predicate_name);
        return false;
    }
    if (domain->predicates[predicate_index].var_count != fact->arg_count) {
        char *predicate_name = StringAlloc(&fact->predicate);
        ERROR(
            "Predicate %s has %d variables, but problem has fact that provides %d arguments",
            predicate_name, domain->predicates[predicate_index].var_count, fact->arg_count
        );
        free(predicate_name);
        return false;
    }
    for (uint i = 0; i < fact->arg_count; i++) {
        if (ProblemObjectIndex(problem, &fact->args[i]) == UINT_MAX) {
            char *arg = StringAlloc(&fact->args[i]);
            ERROR("Fact in problem refers to undefined object %s", arg);
            free(arg);
            return false;
        }
    }
    return true;
}

bool VerifyFacts(const Domain *domain, const Problem *problem, const SFact *facts, uint len) {
    for (uint i = 0; i < len; i++)
        if (!VerifyFact(domain, problem, &facts[i])) return false;
    return true;
}

bool VerifyProblem(const Domain *domain, const Problem *problem) {
    return VerifyFacts(domain, problem, problem->inits, problem->init_count) &&
           VerifyFacts(domain, problem, problem->goals, problem->goal_count);
}
