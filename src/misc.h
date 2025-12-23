#ifndef STRING_H
#define STRING_H

#include <stddef.h>

typedef struct {
	const char* ptr;
	size_t      len;
} string;

#endif
