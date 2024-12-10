#include <limits.h>

#include "log.h"
#include "verify.h"

bool VerifyFact(const Domain *domain, const Problem *problem, const SFact *fact) {
    const uint predicate_index = DomainPredicateIndex(domain, &fact->predicate);
    if (predicate_index == UINT_MAX) {
        ERROR(
            "Problem contains fact that referes to undefined predicate %.*s", 
            fact->predicate.len, fact->predicate.ptr
        );
        return false;
    }
    if (domain->predicates[predicate_index].var_count != fact->arg_count) {
        ERROR(
            "Predicate %.*s has %d variables, but problem has fact that provides %d arguments",
            fact->predicate.len, fact->predicate.ptr,
            domain->predicates[predicate_index].var_count, fact->arg_count
        );
        return false;
    }
    for (uint i = 0; i < fact->arg_count; i++) {
        if (ProblemObjectIndex(problem, &fact->args[i]) == UINT_MAX) {
            ERROR(
                "Fact in problem refers to undefined object %.*s",
                fact->args[i].len, fact->args[i].ptr
            );
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
