#include "misc.h"
#include <stdlib.h>
#include <string.h>

void memswap(void *restrict buffer, void *l, void *r, size_t size) {
    memcpy(buffer, l, size);
    memmove(l, r, size);
    memcpy(r, buffer, size);
}

bool StringCmp(const string *a, const string *b) { return a->len == b->len && strncmp(a->ptr, b->ptr, a->len) == 0; }

char* StringAlloc(const string *str) {
    char *_str = malloc(str->len + 1);
    strncpy(_str, str->ptr, str->len);
    _str[str->len] = '\0';
    return _str;
}
