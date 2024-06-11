#include "parsing/domain/domain.h"
#include "parsing/domain/lexer.h"
#include <stdio.h>

int main(void) {
    Expression exp = {.kind = FACT, .data = {.FACT.predicate = 0, .FACT.parameter = 1}};
    printf("%d %d %d\n", exp.kind, exp.data.UNARY.child->kind, exp.data.FACT.parameter);
    return 0;
}
