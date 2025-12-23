#ifndef STRING_H
#define STRING_H

#include <stddef.h>

typedef struct {
	const char* ptr;
	size_t      len;
} string;

size_t slen(const string* arr);
const string* sfind(const string* arr, const string* s);

#endif
