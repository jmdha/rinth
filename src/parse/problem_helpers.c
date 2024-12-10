#include <limits.h>
#include <stdio.h>

#include "problem.h"

void ProblemPrint(Problem *problem) {
    printf("Domain: %.*s\n", problem->domain.len, problem->domain.ptr);
    printf("Name: %.*s\n", problem->name.len, problem->name.ptr);
    printf("Object count: %d\n", problem->object_count);
    printf("Objects:\n");
    for (uint i = 0; i < problem->object_count; i++)
        printf("\t%.*s\n", problem->objects[i].len, problem->objects[i].ptr);
    printf("Init count: %d\n", problem->init_count);
    printf("Inits:\n");
    for (uint i = 0; i < problem->init_count; i++) {
        printf("\t%.*s", problem->inits[i].predicate.len, problem->inits[i].predicate.ptr);
        for (uint t = 0; t < problem->inits[i].arg_count; t++)
            printf(" %.*s", problem->inits[i].args[t].len, problem->inits[i].args[t].ptr);
        printf("\n");
    }
    printf("Goal count: %d\n", problem->goal_count);
    printf("Goals:\n");
    for (uint i = 0; i < problem->goal_count; i++) {
        printf("\t%.*s", problem->goals[i].predicate.len, problem->goals[i].predicate.ptr);
        for (uint t = 0; t < problem->goals[i].arg_count; t++)
            printf(" %.*s", problem->goals[i].args[t].len, problem->goals[i].args[t].ptr);
        printf("\n");
    }
}

uint ProblemObjectIndex(const Problem *problem, const string *str) {
    for (unsigned int i = 0; i < problem->object_count; i++)
        if (str_cmp(str, &problem->objects[i])) return i;
    return UINT_MAX;
}
