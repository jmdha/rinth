#include <limits.h>

#include "log.h"
#include "verify.h"

bool VerifyExpression(const Domain *domain, const Action *action, const Expression *exp) {
    switch (exp->kind) {
    case E_NOT: return VerifyExpression(domain, action, exp->data.unary);
    case E_OR:
    case E_AND:
        for (uint i = 0; i < exp->data.nary.count; i++)
            if (!VerifyExpression(domain, action, exp->data.nary.exps[i])) return false;
        return true;
    case E_ATOM:
        if (DomainPredicateIndex(domain, &exp->data.atom.predicate) == UINT_MAX) {
            ERROR(
                "Action %s refers to undefined predicate %s.", StringAlloc(&action->name),
                StringAlloc(&exp->data.atom.predicate)
            );
            return false;
        }
        for (uint i = 0; i < exp->data.atom.var_count; i++)
            if (ActionVarIndex(action, &exp->data.atom.vars[i]) == UINT_MAX) {
                ERROR(
                    "Action %s refers to undefined variable %s.", StringAlloc(&action->name),
                    StringAlloc(&exp->data.atom.vars[i])
                );
                return false;
            }
        return true;
    }
}

bool VerifyAction(const Domain *domain, const Action *action) {
    return VerifyExpression(domain, action, action->precondition) &&
           VerifyExpression(domain, action, action->effect);
}

bool VerifyDomain(const Domain *domain) {
    for (unsigned int i = 0; i < domain->action_count; i++)
        if (!VerifyAction(domain, &domain->actions[i])) return false;
    return true;
}
