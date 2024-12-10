#include <assert.h>
#include <memory.h>

#include "fact.h"

struct fact fact(u8 predicate, u8 len, u8* args) {
    struct fact fact;
    fact.predicate = predicate;
    fact.len = len;
    memcpy(fact.args, args, len);
    return fact;
}

bool fact_equal(const struct fact *a, const struct fact *b) {
    if (a->predicate != b->predicate) return false;
    assert(a->len == b->len);
    for (uint i = 0; i < a->len; i++)
        if (a->args[i] != b->args[i]) return false;
    return true;
}
