#include "io.h"
#include <stdio.h>
#include <time.h>

int main(void) {
    clock_t start    = clock();
    const char *path = "domain.pddl";
    File file        = FileOpen(path);
    FileClose(&file);
    clock_t end   = clock();
    float elapsed = (float)(end - start) / CLOCKS_PER_SEC;
    printf("Time %f s\n", elapsed);
}
