#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "state_heap.h"
#include "log.h"

typedef struct {
	size_t  len;
	size_t  cap;
	state** arr;
} bucket;

struct state_heap {
	size_t  len;
	size_t  cap;
	bucket* arr;
};

state_heap* sh_new(void) {
	state_heap* sh = malloc(sizeof(state_heap));
	if (!sh)
		exit(errno);

	sh->len = 0;
	sh->cap = 8;
	sh->arr = malloc(sh->cap * sizeof(bucket));
	if (!sh->arr)
		exit(errno);
	for (size_t i = 0; i < sh->cap; i++) {
		bucket* b = &sh->arr[i];
		b->len = 0;
		b->cap = 8;
		b->arr = malloc(b->cap * sizeof(bucket));
		if (!b->arr)
			exit(errno);
	}

	return sh;
}

void sh_free(state_heap* sh) {
	for (size_t i = 0; i < sh->cap; i++) {
		for (size_t t = 0; t < sh->arr[i].len; t++)
			state_free(sh->arr[i].arr[t]);
		free(sh->arr[i].arr);
	}
	free(sh->arr);
	free(sh);
}

bool sh_empty(const state_heap* sh) {
	return sh_size(sh) == 0;
}

size_t sh_size(const state_heap* sh) {
	return sh->len;
}

void sh_push(state_heap* sh, state* s, size_t val) {
	if (val >= sh->cap) {
		sh->cap *= 2;
		sh->arr = realloc(sh->arr, sh->cap * sizeof(bucket));
		if (!sh->arr)
			exit(errno);
		INFO("State Heap: %zu", sh->len);
		for (size_t i = sh->cap / 2; i < sh->cap; i++) {
			sh->arr[i].len = 0;
			sh->arr[i].cap = 8;
			sh->arr[i].arr = malloc(sh->arr[i].cap * sizeof(state*));
			if (!sh->arr[i].arr)
				exit(errno);
		}
	}

	bucket* b = &sh->arr[val];

	if (b->len >= b->cap) {
		b->cap *= 2;
		b->arr = realloc(b->arr, b->cap * sizeof(state*));
		if (!b->arr)
			exit(errno);
	}

	b->arr[b->len++] = s;
	sh->len++;
}

state* sh_pop(state_heap* sh) {
	for (int i = sh->cap - 1; i >= 0; i--)
		if (sh->arr[i].len > 0) {
			sh->len--;
			return sh->arr[i].arr[--sh->arr[i].len];
		}
	return NULL;
}
