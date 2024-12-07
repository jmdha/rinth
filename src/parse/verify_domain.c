#include <limits.h>
#include <stdlib.h>

#include "log.h"
#include "verify.h"

bool VerifyExpression(const Domain *domain, const Action *action, const Expression *exp) {
    switch (exp->kind) {
    case E_NOT: return VerifyExpression(domain, action, exp->data.unary);
    case E_OR:
    case E_AND:
        for (uint i = 0; i < exp->data.nary.count; i++)
            if (!VerifyExpression(domain, action, exp->data.nary.exps[i])) return false;
        break;
    case E_ATOM:
        if (DomainPredicateIndex(domain, &exp->data.atom.predicate) == UINT_MAX) {
            char *action_name    = StringAlloc(&action->name);
            char *predicate_name = StringAlloc(&exp->data.atom.predicate);
            ERROR("Action %s refers to undefined predicate %s.", action_name, predicate_name);
            free(action_name);
            free(predicate_name);
            return false;
        }
        for (uint i = 0; i < exp->data.atom.var_count; i++)
            if (ActionVarIndex(action, &exp->data.atom.vars[i]) == UINT_MAX) {
                char *action_name   = StringAlloc(&action->name);
                char *variable_name = StringAlloc(&exp->data.atom.vars[i]);
                ERROR("Action %s refers to undefined variable %s.", action_name, variable_name);
                free(action_name);
                free(variable_name);
                return false;
            }
        break;
    }
    return true;
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
