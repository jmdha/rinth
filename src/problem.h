#pragma once

#include "bounds.h"
#include "misc.h"

typedef struct {
    string predicate;
    uint arg_count;
    string args[MAX_VARIABLES];
} SFact;

typedef struct {
    string domain;
    string name;
    uint object_count;
    string objects[MAX_OBJECTS];
    uint init_count;
    SFact inits[MAX_INITS];
    uint goal_count;
    SFact goals[MAX_GOALS];
} Problem;

void problem_parse_(Problem *problem, const char *str);
Problem problem_parse(const char *str);
void problem_print(Problem *problem);
