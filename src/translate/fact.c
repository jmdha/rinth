#include <assert.h>
#include <stdlib.h>

#include "fact.h"

struct fact fact() {
    abort();
}

bool fact_equal(const struct fact *a, const struct fact *b) {
    if (a->predicate != b->predicate) return false;
    assert(a->len == b->len);
    for (uint i = 0; i < a->len; i++)
        if (a->args[i] != b->args[i]) return false;
    return true;
}
