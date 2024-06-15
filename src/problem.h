#pragma once

#define MAX_OBJECTS 1000
#define MAX_ARGS 8
#define MAX_INITS 1000
#define MAX_GOALS 1000

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

// TODO: Reduce init time. Currently a large portion of the time is taken initializing struct.
// A likely solution would be to use lists
Problem ProblemParse(const char *str);
void ProblemDelete(Problem *problem);

void ProblemPrint(Problem *problem);
