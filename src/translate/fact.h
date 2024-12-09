#pragma once

#include "misc.h"

struct fact {
    u8 predicate;
    u8 len;
    u8 args[4];
};

struct fact fact();
bool fact_equal(const struct fact *a, const struct fact *b);
