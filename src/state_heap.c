#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "state_heap.h"
#include "state_set.h"
#include "log.h"

struct state_heap {
	size_t      cap;
	state_set** arr;
};

state_heap* sh_new(void) {
	state_heap* sh = malloc(sizeof(state_heap));
	if (!sh)
		exit(errno);

	sh->cap = 64;
	sh->arr = malloc(sh->cap * sizeof(state_set*));
	if (!sh->arr)
		exit(errno);
	for (size_t i = 0; i < sh->cap; i++)
		sh->arr[i] = ss_new();

	return sh;
}

void sh_free(state_heap* sh) {
	for (size_t i = 0; i < sh->cap; i++)
		ss_free(sh->arr[i]);
	free(sh->arr);
	free(sh);
}

size_t sh_size(const state_heap* sh) {
	size_t size = sizeof(state_heap);

	for (size_t i = 0; i < sh->cap; i++)
		if (sh->arr[i])
			size += ss_size(sh->arr[i]);

	return size;
}

size_t sh_count(const state_heap* sh) {
	size_t count = 0;
	for (size_t i = 0; i < sh->cap; i++)
		if (sh->arr[i])
			count += ss_count(sh->arr[i]);
	return count;
}

bool sh_empty(const state_heap* sh) {
	return sh_count(sh) == 0;
}

void sh_push(state_heap* sh, state* s, size_t val) {
	if (val >= sh->cap) {
		sh->cap *= 2;
		sh->arr = realloc(sh->arr, sh->cap * sizeof(state_set*));
		if (!sh->arr)
			exit(errno);
		for (size_t i = sh->cap / 2; i < sh->cap; i++)
			sh->arr[i] = ss_new();
	}
	ss_add(sh->arr[val], s);
}

state* sh_pop(state_heap* sh) {
	for (int i = sh->cap - 1; i >= 0; i--)
		if (!ss_empty(sh->arr[i]))
			return ss_pop(sh->arr[i]);
	return NULL;
}

void sh_reduce(state_heap* sh, size_t count) {
	size_t ccount = sh_count(sh);
	for (size_t i = 0; i < sh->cap; i++) {
		while (!ss_empty(sh->arr[i])) {
			if (ccount < count)
				break;
			state_free(ss_pop(sh->arr[i]));
			ccount--;
		}
	}
}
