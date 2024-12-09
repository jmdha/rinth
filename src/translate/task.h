#pragma once

#include "parse/domain.h"
#include "parse/problem.h"
#include "translate/scheme.h"

#define MAX_SCHEMES 16

typedef struct {
    uint predicate_count;
    string predicates[MAX_PREDICATES];
    //uint object_count;
    //string objects[MAX_OBJECTS];
    //uint scheme_count;
    //Scheme schemes[MAX_SCHEMES];
    //uint init_count;
    //Fact inits[MAX_INITS];
    //uint goal_count;
    //Fact goals[MAX_GOALS];
} Task;

Task Translate(const Domain *domain, const Problem *problem);
