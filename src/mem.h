#ifndef MEM_H
#define MEM_H

#include <stddef.h>

// Simple wrappers that includes handling of failed allocation

// malloc wrapper that exists in case of failed allocation
void* malloc_(size_t size);

// calloc wrapper that exists in case of failed allocation
void* calloc_(size_t num, size_t size);

// realloc wrapper that exists in case of failed allocation
void* realloc_(void* ptr, size_t size);

#endif
