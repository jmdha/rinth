#include "io.h"
#include "problem.h"
#include <stdio.h>
#include <time.h>

int main(int argc, char **argv) {
    if (argc < 2) return 1;
    const char *path = argv[1];
    File file        = FileOpen(path);
    clock_t start    = clock();
    Problem problem  = ProblemParse(file.buffer);
    clock_t end      = clock();
    double seconds   = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Parsing took: %f s\n", seconds);
    // ProblemPrint(&problem);
    FileClose(&file);
    ProblemDelete(&problem);
    printf("Throughput: %f MB/s \n", (double)file.len * 0.000001 / seconds);
    return 0;
}
