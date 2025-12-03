#pragma once

#include "state.h"

typedef struct statespace statespace;
typedef struct statespace_iter statespace_iter;

statespace* statespace_new(
	void
);

size_t statespace_count(
	const statespace* ss
);

bool statespace_contains(
	const statespace* ss,
	const state* s
);

void statespace_add(
	statespace* ss,
	state* s
);
