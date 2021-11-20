/**
 * @file log.c
 * @brief Logging functions definitions
 * @version 0.1
 * @date 2021-11-17
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "log.h"

log_level_t log_level = LOG_LEVEL_INFO;

const char *log_level_to_color(log_level_t level)
{
    switch (level)
    {
    case LOG_LEVEL_VERBOSE:
        return COLOR_GREY;
    case LOG_LEVEL_DEBUG:
        return COLOR_RESET;
    case LOG_LEVEL_INFO:
        return COLOR_GREEN;
    case LOG_LEVEL_WARNING:
        return COLOR_YELLOW;
    case LOG_LEVEL_ERROR:
        return COLOR_RED;
    default:
        return COLOR_CYAN;
    }
}

const char *log_level_to_string(log_level_t level)
{
    switch (level)
    {
    case LOG_LEVEL_VERBOSE:
        return "(V)";
    case LOG_LEVEL_DEBUG:
        return "(D)";
    case LOG_LEVEL_INFO:
        return "(I)";
    case LOG_LEVEL_WARNING:
        return "(W)";
    case LOG_LEVEL_ERROR:
        return "(E)";
    default:
        return "(U)";
    }
}

void log_level_set(log_level_t level)
{
    log_level = level;
}

log_level_t log_level_get(void)
{
    return log_level;
}

void log_write(log_level_t level, const char* tag, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    log_write_va(level, tag, format, args);
    va_end(args);
}

void log_write_va(log_level_t level, const char *tag, const char *format, va_list args)
{
    if (level <= log_level)
    {
        /* TODO: Add timestamp */
        printf("%sCPU1 - %s%s %s: ", COLOR_BLUE, log_level_to_color(level), log_level_to_string(level), tag);
        vprintf(format, args);
        printf("%s", COLOR_RESET);
    }
}
