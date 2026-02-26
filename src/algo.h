#ifndef ALGO_H
#define ALGO_H

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct cp_iter cp_iter;

cp_iter *cp_init(size_t **vals);
void cp_free(cp_iter *cpi);
bool cp_step(cp_iter *cpi, size_t *vals);

#endif
