#include "misc.h"
#include "log.h"
#include <stdlib.h>
#include <string.h>

void memswap(void *restrict buffer, void *l, void *r, size_t size) {
    memcpy(buffer, l, size);
    memmove(l, r, size);
    memcpy(r, buffer, size);
}

bool StringCmp(const void *a, const void *b) {
    return ((const string *)a)->len == ((const string *)b)->len &&
           strncmp(((const string *)a)->ptr, ((const string *)b)->ptr, ((const string *)a)->len) == 0;
}

uint IndexOf(const void *list, uint len, const void *elem, size_t s, bool (*cmp)(const void *, const void *)) {
    for (uint i = 0; i < len; i++)
        if (cmp((const char*)list + i * s, elem)) return i;
    FATAL("Failed to find elem in list");
    abort();
}
