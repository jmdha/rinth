#pragma once

#define MAX_OBJECTS 500
#define MAX_ARGS 4
#define MAX_INITS 500
#define MAX_GOALS 500

typedef struct {
    char *predicate;
    int arg_count;
    char *args[MAX_ARGS];
} Fact;

typedef struct {
    char *domain;
    char *name;
    int object_count;
    char *objects[MAX_OBJECTS];
    int init_count;
    Fact inits[MAX_INITS];
    int goal_count;
    Fact goals[MAX_GOALS];
} Problem;

Problem ProblemParse(const char *str);
void ProblemDelete(Problem *problem);

void ProblemPrint(Problem *problem);
