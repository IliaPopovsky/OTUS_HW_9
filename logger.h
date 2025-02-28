#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

#include <stdio.h>
#include <string.h>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define LOG_DEBUG(fmt, ...) logger_log(LogLevel_DEBUG, __FILENAME__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)  logger_log(LogLevel_INFO , __FILENAME__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)  logger_log(LogLevel_WARN , __FILENAME__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) logger_log(LogLevel_ERROR, __FILENAME__, __LINE__, fmt, ##__VA_ARGS__)

typedef enum {
    LogLevel_DEBUG,
    LogLevel_INFO,
    LogLevel_WARN,
    LogLevel_ERROR,
} LogLevel;

/**
 * Initialize the logger as a console logger.
 * If the file pointer is NULL, stdout will be used.
 *
 * @param[in] output A file pointer. Make sure to set stdout or stderr.
 * @return Non-zero value upon success or 0 on error
 */
int logger_initConsoleLogger(FILE* output);

/**
 * Initialize the logger as a file logger.
 * If the filename is NULL, return without doing anything.
 *
 * @param[in] filename The name of the output file
 * @param[in] maxFileSize The maximum number of bytes to write to any one file
 * @param[in] maxBackupFiles The maximum number of files for backup
 * @return Non-zero value upon success or 0 on error
 */

 /**
 * Set the log level.
 * Message levels lower than this value will be discarded.
 * The default log level is INFO.
 *
 * @param[in] level A log level
 */
void logger_setLevel(LogLevel level);

/**
 * Get the log level that has been set.
 * The default log level is INFO.
 *
 * @return The log level
 */
LogLevel logger_getLevel(void);

/**
 * Check if a message of the level would actually be logged.
 *
 * @return Non-zero value if the log level is enabled
 */
#endif // LOGGER_H_INCLUDED
