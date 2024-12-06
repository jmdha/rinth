#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef unsigned int uint;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef struct {
    const char *ptr;
    uint        len;
} string;

void memswap(void *restrict buffer, void *l, void *r, size_t size);

#define swap(a, b)                                                                                 \
    memswap(&(struct {                                                                             \
        _Static_assert(sizeof *(a) == sizeof *(b), "arguments of swap must have same size");       \
        char _[sizeof *(a)];                                                                       \
    }){0},                                                                                         \
            (a), (b), sizeof *(a))

#define max(a, b)                                                                                  \
    ({                                                                                             \
        __typeof__(a) _a = (a);                                                                    \
        __typeof__(b) _b = (b);                                                                    \
        _a > _b ? _a : _b;                                                                         \
    })

#define min(a, b)                                                                                  \
    ({                                                                                             \
        __typeof__(a) _a = (a);                                                                    \
        __typeof__(b) _b = (b);                                                                    \
        _a < _b ? _a : _b;                                                                         \
    })

bool StringCmp(const string *a, const string *b);
char* StringAlloc(const string *str);
