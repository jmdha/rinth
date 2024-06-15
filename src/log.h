#pragma once

#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1

#ifdef LOG_DEBUG
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1
#elif LOG_TRACE
#define LOG_DEBUG_ENABLED 0
#define LOG_TRACE_ENABLED 1
#else
#define LOG_DEBUG_ENABLED 0
#define LOG_TRACE_ENABLED 0
#endif

typedef enum LogLevel {
    LOG_LEVEL_FATAL = 0,
    LOG_LEVEL_ERROR = 1,
    LOG_LEVEL_WARN  = 2,
    LOG_LEVEL_INFO  = 3,
    LOG_LEVEL_DEBUG = 4,
    LOG_LEVEL_TRACE = 5
} LogLevel;

void LogInit(void);
void LogStop(void);

void _LogOutput(LogLevel level, const char *msg, ...);

#define FATAL(msg, ...) _LogOutput(LOG_LEVEL_FATAL, msg, ##__VA_ARGS__);
#define ERROR(msg, ...) _LogOutput(LOG_LEVEL_ERROR, msg, ##__VA_ARGS__);

#if LOG_WARN_ENABLED
#define WARN(msg, ...) _LogOutput(LOG_LEVEL_WARN, msg, ##__VA_ARGS__);
#else
#define WARN(msg, ...)
#endif

#if LOG_INFO_ENABLED
#define INFO(msg, ...) _LogOutput(LOG_LEVEL_INFO, msg, ##__VA_ARGS__);
#else
#define INFO(msg, ...)
#endif

#if LOG_DEBUG_ENABLED
#define DEBUG(msg, ...) _LogOutput(LOG_LEVEL_DEBUG, msg, ##__VA_ARGS__);
#else
#define DEBUG(msg, ...)
#endif

#if LOG_TRACE_ENABLED
#define TRACE(msg, ...) _LogOutput(LOG_LEVEL_TRACE, msg, ##__VA_ARGS__);
#else
#define TRACE(msg, ...)
#endif
