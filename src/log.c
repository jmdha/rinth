#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "log.h"

// clang-format off
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define GRAY    "\x1b[90m"
#define RESET   "\x1b[0m"
// clang-format on

clock_t program_start;

void log_init(void) {
    program_start = clock();
}

void log_out(int level, const char *msg, ...) {
    char formatted[32000];
    memset(formatted, 0, sizeof(formatted));

    va_list arg_ptr;
    va_start(arg_ptr, msg);
    vsnprintf(formatted, 32000, msg, arg_ptr);
    va_end(arg_ptr);

    double time_stamp = (double)(clock() - program_start) / CLOCKS_PER_SEC;

    char out[33000];
    if (level == LOG_LEVEL_TRACE)
        sprintf(out, GRAY "[%8.4fs] %s%s\n", time_stamp, formatted, RESET);
    else
        sprintf(out, "[%8.4fs] %s\n", time_stamp, formatted);

    fprintf(stdout, "%s", out);
}
