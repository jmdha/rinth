#pragma once

#include <stdbool.h>

#include "misc.h"
#include "parse/domain.h"

typedef struct {
    uint predicate;
    uint *v_args;
    bool val;
} Atom;

typedef struct {
    char *name;
    Atom *v_pre;
    Atom *v_eff;
} Scheme;

Scheme TranslateAction(char **v_predicates, Action *action);
Scheme *TranslateActions(char **v_predicates, Action *actions, uint count);
