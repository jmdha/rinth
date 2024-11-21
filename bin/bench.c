#include <stdio.h>
#include <time.h>
#define STB_DS_IMPLEMENTATION
#include <stb_ds.h>

#include "io.h"
#include "log.h"
#include "parse/domain.h"
#include "parse/problem.h"
#include "translate/task.h"

int main(int argc, char **argv) {
    LogInit();
    const char *problem_path = argv[1];
    File problem_file = FileOpen(problem_path);
    printf("Bytes: %f MB\n", problem_file.len / (float) 1024 / 1024);
    clock_t start = clock();
    for (int i = 0; i < 100000; i++) {
        Problem problem = ProblemParse(problem_file.buffer);
        ProblemDelete(&problem);
    }
    clock_t end = clock();
    float secs = (float)(end - start) / CLOCKS_PER_SEC;
    printf("%f MB/s\n", (problem_file.len / (float) 1024 / 1024) * 100000 / secs);
    return 0;
}
