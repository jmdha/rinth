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

typedef enum LogLevel {
    LOG_LEVEL_ERROR = 0,
    LOG_LEVEL_INFO  = 1,
    LOG_LEVEL_TRACE = 2
} LogLevel;

void LogInit(void);
void LogStop(void);

void _LogOutput(LogLevel level, const char *msg, ...);

#if LOG_ERROR_ENABLED
#define ERROR(msg, ...) _LogOutput(LOG_LEVEL_ERROR, msg, ##__VA_ARGS__);
#else
#define ERROR(msg, ...)
#endif

#if LOG_INFO_ENABLED
#define INFO(msg, ...) _LogOutput(LOG_LEVEL_INFO, msg, ##__VA_ARGS__);
#else
#define INFO(msg, ...)
#endif

#if LOG_TRACE_ENABLED
#define TRACE(msg, ...) _LogOutput(LOG_LEVEL_TRACE, msg, ##__VA_ARGS__);
#else
#define TRACE(msg, ...)
#endif
