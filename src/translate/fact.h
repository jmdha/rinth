#pragma once

#include "misc.h"
#include "bounds.h"

struct fact {
    u8 predicate;
    u8 len;
    u8 args[MAX_VARIABLES];
};

struct fact fact(u8 predicate, u8 len, u8* args);
bool fact_equal(const struct fact *a, const struct fact *b);
