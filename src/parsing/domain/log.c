#include "log.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void LogInit(void) {
    // TODO: Create log file
}

void LogStop(void) {
    // TODO: Perform any cleanup
}

void _LogOutput(LogLevel level, const char *msg, ...) {
    const char *LEVELS[6] = {"[FATAL]", "[ERROR]", "[WARN]", "[INFO]", "[DEBUG]", "[TRACE]"};

    bool is_error = level < 2;

    char formatted[32000];
    memset(formatted, 0, sizeof(formatted));

    va_list arg_ptr;
    va_start(arg_ptr, msg);
    vsnprintf(formatted, 32000, msg, arg_ptr);
    va_end(arg_ptr);

    char out[33000];
    sprintf(out, "%s %s\n", LEVELS[level], formatted);

    if (is_error)
        fprintf(stderr, "%s", out);
    else
        fprintf(stdout, "%s", out);
}
