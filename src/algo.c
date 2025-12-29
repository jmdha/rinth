#include <memory.h>
#include <stdint.h>

#include "algo.h"

struct cp_iter {
        size_t** root;
        size_t*  offsets;
};

void cp_free(cp_iter* cpi) {
        free(cpi->offsets);
        free(cpi);
}

cp_iter* cp_init(size_t** vals) {
        cp_iter* cpi = malloc(sizeof(cp_iter));
        cpi->root    = vals;
        size_t len   = 0;
        for (size_t i = 0; cpi->root[i]; i++)
                len++;
        cpi->offsets = calloc(len, sizeof(size_t));
        return cpi;
}

bool cp_propagate(cp_iter* cpi) {
        for (size_t i = 1; cpi->root[i - 1] && cpi->root[i]; i++) {
                if (cpi->root[i][cpi->offsets[i]] != SIZE_MAX &&
                    cpi->root[i][cpi->offsets[i] + 1] != SIZE_MAX) {
                        cpi->offsets[i]++;
                        for (size_t t = 1; t <= i; t++)
                                cpi->offsets[i - t] = 0;
                        return true;
                }
        }
        return false;
}

bool cp_step(size_t* vals, cp_iter* cpi) {
        if (!cpi->root[0])
                return false;
        if (cpi->root[0][cpi->offsets[0]] == SIZE_MAX)
                if (!cp_propagate(cpi))
                        return false;
        for (size_t i = 0; cpi->root[i]; i++)
                vals[i] = cpi->root[i][cpi->offsets[i]];
        cpi->offsets[0]++;
        return true;
}
