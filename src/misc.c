#include <string.h>

#include "misc.h"

bool str_contains(const string* str, const string* list, uint count) {
    for (uint i = 0; i < count; i++)
        if (str_cmp(str, &list[i]))
            return true;
    return false;
}

bool str_cmp(const string *a, const string *b) {
    return a->len == b->len && strncmp(a->ptr, b->ptr, a->len) == 0;
}

bool str_cmp_s(const string *a, const char *b) {
    return a->len == strlen(b) && strncmp(a->ptr, b, a->len) == 0;
}
