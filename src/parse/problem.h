#pragma once

#include "misc.h"

#define MAX_OBJECTS 256
#define MAX_ARGS 8
#define MAX_INITS 256
#define MAX_GOALS 256

typedef struct {
    string predicate;
    uint   arg_count;
    string args[MAX_ARGS];
} SFact;

typedef struct {
    string domain;
    string name;
    uint   object_count;
    string objects[MAX_OBJECTS];
    uint   init_count;
    SFact  inits[MAX_INITS];
    uint   goal_count;
    SFact  goals[MAX_GOALS];
} Problem;

void ProblemParse(Problem *problem, const char *str);
void ProblemPrint(Problem *problem);
