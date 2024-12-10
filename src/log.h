#pragma once

#ifdef LOG_ERROR
#define LOG_ERROR_ENABLED 1
#else
#define LOG_ERROR_ENABLED 0
#endif
#ifdef LOG_ERROR
#define LOG_INFO_ENABLED 1
#else
#define LOG_INFO_ENABLED 0
#endif
#ifdef LOG_TRACE
#define LOG_TRACE_ENABLED 1
#else
#define LOG_TRACE_ENABLED 0
#endif

#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_INFO  1
#define LOG_LEVEL_TRACE 2

void log_init(void);
void log_out(int level, const char *msg, ...);

#if LOG_ERROR_ENABLED
#define ERROR(msg, ...) log_out(LOG_LEVEL_ERROR, msg, ##__VA_ARGS__);
#else
#define ERROR(msg, ...)
#endif

#if LOG_INFO_ENABLED
#define INFO(msg, ...) log_out(LOG_LEVEL_INFO, msg, ##__VA_ARGS__);
#else
#define INFO(msg, ...)
#endif

#if LOG_TRACE_ENABLED
#define TRACE(msg, ...) log_out(LOG_LEVEL_TRACE, msg, ##__VA_ARGS__);
#else
#define TRACE(msg, ...)
#endif
