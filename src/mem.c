#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#include "mem.h"

void* malloc_(size_t size) {
	void* ptr = malloc(size);
	if (!ptr) {
		fprintf(stderr, "%zu: failed to allocate memory.\n", size);
		exit(errno);
	}
	return ptr;
}

void* calloc_(size_t num, size_t size) {
	void* ptr = calloc(num, size);
	if (!ptr) {
		fprintf(stderr, "%zu: failed to allocate memory.\n", size);
		exit(errno);
	}
	return ptr;
}

void* realloc_(void* ptr, size_t size) {
	ptr = realloc(ptr, size);
	if (!ptr) {
		fprintf(stderr, "%zu: failed to allocate memory.\n", size);
		exit(errno);
	}
	return ptr;
}
