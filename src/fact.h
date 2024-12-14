#pragma once

#include "misc.h"
#include "bounds.h"

struct fact {
    uint predicate;
    u8 len;
    u8 args[MAX_VARIABLES];
};

bool fact_equal(const struct fact *a, const struct fact *b);
