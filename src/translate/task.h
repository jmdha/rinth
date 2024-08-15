#pragma once

#include "parse/domain.h"
#include "parse/problem.h"
#include "translate/operator.h"

typedef struct {
    char **v_predicates;
    char **v_objects;
    Operator *v_operators;
    Fact *v_init;
    Fact *v_goal;
} Task;

Task Translate(Domain *domain, Problem *problem);
void TaskDelete(Task *task);
