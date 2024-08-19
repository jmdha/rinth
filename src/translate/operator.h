#pragma once

#include "misc.h"

typedef struct {
    char *name;
    uint *v_pre;
    uint *v_eff;
} Operator;

void OperatorFree(Operator *operator);
