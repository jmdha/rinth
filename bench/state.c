#include <ubench/ubench.h>

#include "state.h"

static void random_fact(size_t* pred, size_t* len, size_t* args) {
	*pred = rand() % 16;
	*len = rand() % 2;
	for (size_t i = 0; i < *len; i++)
		args[i] = rand() % 64;
}

UBENCH_EX(state, insert) {
	state* s = state_new();
        UBENCH_DO_BENCHMARK() {
        	for (uint i = 0; i < 100000; i++) {
			size_t pred;
			size_t len;
			size_t args[32];
			random_fact(&pred, &len, args);
			state_insert(s, pred, len, args);
		}
	}
	state_free(s);
}

UBENCH_EX(state, mixed) {
	state* s = state_new();
        UBENCH_DO_BENCHMARK() {
        	for (uint i = 0; i < 100000; i++) {
			size_t pred;
			size_t len;
			size_t args[32];
			random_fact(&pred, &len, args);
			state_insert(s, pred, len, args);
			random_fact(&pred, &len, args);
			state_remove(s, pred, len, args);
		}
	}
	state_free(s);
}

UBENCH_EX(state, contains) {
	state* s = state_new();
        for (uint i = 0; i < 1000; i++) {
		size_t pred;
		size_t len;
		size_t args[32];
		random_fact(&pred, &len, args);
		state_insert(s, pred, len, args);
	}
        UBENCH_DO_BENCHMARK() {
        	for (uint i = 0; i < 100000; i++) {
			size_t pred;
			size_t len;
			size_t args[32];
			random_fact(&pred, &len, args);
			state_contains(s, pred, len, args);
		}
	}
	state_free(s);
}
