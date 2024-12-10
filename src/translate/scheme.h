#pragma once

#include <stdbool.h>

#include "bounds.h"
#include "misc.h"

typedef struct {
    uint predicate;
    uint arg_count;
    uint args[MAX_VARIABLES];
    bool val;
} Atom;

typedef struct {
    string name;
    uint pre_count;
    Atom pre[MAX_ATOMS];
    uint eff_count;
    Atom eff[MAX_ATOMS];
} Scheme;
