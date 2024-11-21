#pragma once

#include "misc.h"

#define MAX_OBJECTS 500
#define MAX_ARGS 4
#define MAX_INITS 500
#define MAX_GOALS 500

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

Problem ProblemParse(const char *str);

void ProblemPrint(Problem *problem);
