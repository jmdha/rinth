#include <string.h>

#include "misc.h"

size_t slen(const string* arr) {
	const string* o = arr;
	while (arr->ptr) 
		arr++;
	return arr - o;
}

const string* sfind(const string* arr, const string* s) {
	while (arr->ptr)
		if (arr->len == s->len && strncmp(arr->ptr, s->ptr, s->len) == 0)
			return arr;
		else
			arr++;
	return NULL;
}
