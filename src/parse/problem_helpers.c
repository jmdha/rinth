#include <limits.h>

#include "problem.h"

uint ProblemObjectIndex(const Problem *problem, const string *str) {
    for (unsigned int i = 0; i < problem->object_count; i++)
        if (StringCmp(str, &problem->objects[i])) return i;
    return UINT_MAX;
}
