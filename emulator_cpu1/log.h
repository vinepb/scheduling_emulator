/**
 * @file log.h
 * @brief Logging functions declarations
 * @version 0.1
 * @date 2021-11-17
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdarg.h>

#define USE_COLOR     0
#define GLOBAL_LOG_LEVEL LOG_LEVEL_VERBOSE

#if USE_COLOR
#define COLOR_GREY    "\033[1;30m"
#define COLOR_RED     "\033[0;31m"
#define COLOR_GREEN   "\033[0;32m"
#define COLOR_YELLOW  "\033[1;33m"
#define COLOR_BLUE    "\033[0;34m"
#define COLOR_MAGENTA "\033[1;35m"
#define COLOR_CYAN    "\033[0;36m"
#define COLOR_RESET   "\033[0m"
#endif

/**
 * @brief Log levels
 */
typedef enum {
    LOG_LEVEL_NONE,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_VERBOSE
} log_level_t;

extern log_level_t log_level;

/**
 * @brief Log level color mapping
 * 
 * @param[in] level Log level
 * @return @c const @c char* Color code
 */
const char *log_level_to_color(log_level_t level);

/**
 * @brief Log level to string
 * 
 * @param[in] level Log level
 * @return @c const @c char* Log level name
 */
const char *log_level_to_string(log_level_t level);

/**
 * @brief Set log level
 * 
 * @param[in] level The log level
 */
void log_level_set(log_level_t level);

/**
 * @brief Get log level
 * 
 * @return @c log_level_t Current log level 
 */
log_level_t log_level_get(void);

/**
 * @brief Write a formatted string to the log
 * 
 * @param[in] level Log level
 * @param[in] tag Local tag
 * @param[in] fmt Format string
 * @param[in] ... Format arguments
 */
void log_write(log_level_t level, const char *tag, const char *fmt, ...)
    __ATTRIBUTE((__format__(__printf__, 3, 4)));

#define LOG_ERROR(tag, fmt, ...)    log_write(LOG_LEVEL_ERROR, tag, fmt"\n", ##__VA_ARGS__)
#define LOG_WARNING(tag, fmt, ...)  log_write(LOG_LEVEL_WARNING, tag, fmt"\n", ##__VA_ARGS__)
#define LOG_INFO(tag, fmt, ...)     log_write(LOG_LEVEL_INFO, tag, fmt"\n", ##__VA_ARGS__)
#define LOG_DEBUG(tag, fmt, ...)    log_write(LOG_LEVEL_DEBUG, tag, fmt"\n", ##__VA_ARGS__)
#define LOG_VERBOSE(tag, fmt, ...)  log_write(LOG_LEVEL_VERBOSE, tag, fmt"\n", ##__VA_ARGS__)

/**
 * @brief Write a formatted string to the log
 * 
 * @param[in] level Log level
 * @param[in] tag Local tag
 * @param[in] fmt Format string
 * @param[in] args Format arguments
 */
void log_write_va(log_level_t level, const char *tag, const char *fmt, va_list args);

#endif // LOG_H
