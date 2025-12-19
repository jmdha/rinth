#pragma once

#define LOG_LEVEL_INFO 1
#define LOG_LEVEL_TRACE 2

// Initialize logging
// Should be the first called thing of the program
// Should only be called once
void log_init(void);

// Internal function used by log macros
// Should never be called directly
void log_out(int level, const char *msg, ...);

#if LOG_INFO
#define INFO(msg, ...) log_out(LOG_LEVEL_INFO, msg, ##__VA_ARGS__);
#else
#define INFO(msg, ...)
#endif

#if LOG_TRACE
#define TRACE(msg, ...) log_out(LOG_LEVEL_TRACE, msg, ##__VA_ARGS__);
#else
#define TRACE(msg, ...)
#endif
