#include <stdio.h>
#include <time.h>
#define STB_DS_IMPLEMENTATION
#include <stb_ds.h>

#include "io.h"
#include "log.h"
#include "parse/problem.h"

int main(int argc, char **argv) {
    LogInit();
    if (argc < 2) {
        ERROR("Missing bench file");
        exit(1);
    }
    const char *problem_path = argv[1];
    File problem_file = FileOpen(problem_path);
    printf("Bytes: %f MB\n", problem_file.len / (float) 1024 / 1024);
    clock_t start = clock();
    for (int i = 0; i < 1000000; i++) {
        ProblemParse(problem_file.buffer);
    }
    clock_t end = clock();
    float secs = (float)(end - start) / CLOCKS_PER_SEC;
    printf("%f MB/s\n", (problem_file.len / (float) 1024 / 1024) * 1000000 / secs);
    return 0;
}
