#ifndef SEARCH_H
#define SEARCH_H

#include <stdint.h>
#include "state.h"

typedef struct {
	size_t len;
	size_t actions[1024];
	size_t args[1024][16];
} path;

path solve(const state* init, const state* goal);
bool reachable(const state* init, const state* goal);

#endif
