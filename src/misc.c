#include <string.h>

#include "misc.h"

bool str_cmp(const string *a, const string *b) {
    return a->len == b->len && strncmp(a->ptr, b->ptr, a->len) == 0;
}

bool str_cmp_s(const string *a, const char *b) {
    return a->len == strlen(b) && strncmp(a->ptr, b, a->len) == 0;
}
