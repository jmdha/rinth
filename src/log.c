#include "log.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

void LogInit(void) {
    program_start = clock();
    // TODO: Create log file
}

void LogStop(void) {
    // TODO: Perform any cleanup
}

void _LogOutput(LogLevel level, const char *msg, ...) {
    const char *LEVELS[6] = {"FATAL", "ERROR", "WARN", "INFO", "DEBUG", "TRACE"};

    bool is_error = level < 2;

    char formatted[32000];
    memset(formatted, 0, sizeof(formatted));

    va_list arg_ptr;
    va_start(arg_ptr, msg);
    vsnprintf(formatted, 32000, msg, arg_ptr);
    va_end(arg_ptr);

    double time_stamp = (double)(clock() - program_start) / CLOCKS_PER_SEC;

    char out[33000];
    if (level == LOG_LEVEL_ERROR)
        sprintf(out, RED "[%-5s %8.4fs] %s\n" RESET, LEVELS[level], time_stamp, formatted);
    else if (level == LOG_LEVEL_WARN)
        sprintf(out, YELLOW "[%-5s %8.4fs] %s\n" RESET, LEVELS[level], time_stamp, formatted);
    else if (level == LOG_LEVEL_TRACE || level == LOG_LEVEL_DEBUG)
        sprintf(out, GRAY "[%-5s %8.4fs] %s\n" RESET, LEVELS[level], time_stamp, formatted);
    else
        sprintf(out, "[%-5s %8.4fs] %s\n", LEVELS[level], time_stamp, formatted);

    if (is_error)
        fprintf(stderr, "%s", out);
    else
        fprintf(stdout, "%s", out);
}
