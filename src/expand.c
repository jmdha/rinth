#include <stdint.h>

#include "expand.h"

void (*f_expand)(const state*);
bool (*f_expand_step)(size_t*, size_t*);

void expand_init(const task* t) {
        expand_init_cp(t);
        expand_init_sqlite(t);

        f_expand      = expand_cp;
        f_expand_step = expand_step_cp;
}

void expand(const state* s) {
        f_expand(s);
}

bool expand_step(size_t* action, size_t* args) {
        return f_expand_step(action, args);
}
