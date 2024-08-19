#include <stb_ds.h>

#include "operator.h"

void OperatorFree(Operator *operator) {
    free(operator->name);
    arrfree(operator->v_pre);
    arrfree(operator->v_eff);
}
