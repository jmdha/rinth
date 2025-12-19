#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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
        uint len;
} string;

// Finds the index of str in list
// Returns UINT_MAX if not found
uint str_index(const string *str, const string *list, uint count);

// Checks whether list contains str
bool str_contains(const string *str, const string *list, uint count);

// Checks whether a and b are of equal length and content
bool str_cmp(const string *a, const string *b);

// Checks whether a and b are of equal length and content
bool str_cmp_s(const string *a, const char *b);
