#pragma once

#include <stdbool.h>

#include "misc.h"
#include "parse/domain.h"

#define MAX_PRECONDITIONS 16
#define MAX_EFFECTS 16

typedef struct {
    uint predicate;
    uint arg_count;
    uint args[MAX_ACTION_VARIABLES];
    bool val;
} Atom;

typedef struct {
    string name;
    uint pre_count;
    Atom pre[MAX_PRECONDITIONS];
    uint eff_count;
    Atom eff[MAX_EFFECTS];
} Scheme;
