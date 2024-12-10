#include <limits.h>

#include "domain.h"
#include "misc.h"

uint DomainPredicateIndex(const Domain *domain, const string *str) {
    for (unsigned int i = 0; i < domain->predicate_count; i++)
        if (str_cmp(str, &domain->predicates[i].name))
            return i;
    return UINT_MAX;
}

uint ActionVarIndex(const Action *action, const string *str) {
    for (unsigned int i = 0; i < action->var_count; i++)
        if (str_cmp(str, &action->vars[i]))
            return i;
    return UINT_MAX;
}
