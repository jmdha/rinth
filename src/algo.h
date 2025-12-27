#ifndef ALGO_H
#define ALGO_H

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct cp_iter cp_iter;

cp_iter* cp_init(size_t** vals);
void     cp_free(cp_iter* cpi);
bool     cp_step(size_t* vals, cp_iter* cpi);

#endif
