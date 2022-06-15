#ifndef __MT_LOG_H__
#define __MT_LOG_H__

#include <mutiny/mutiny.h>

/**
 * Represents a file log.
 */
typedef struct _mt_log {
  // The file to log to.
  FILE* file;
  // Text at the beginning of each message.
  const char* start_txt;
  // The log data.
  char buf[512];
} mt_log_t;

/**
 * @brief Creates a new log.
 * @param file The file to log to.
 * @param start_txt The text at the beginning of each message.
 * @return A new log.
 */
mt_log_t mt_log_init(FILE* file, const char* start_txt);

/**
 * @brief Adds a formatted message to the log using a variable argument list.
 * @param log The log to add the message to.
 * @param fmt The message (Formatted string).
 * @param args The arguments list.
 */
void mt_log_add_v(mt_log_t* log, const char* fmt, va_list args);

/**
 * @brief Adds a formatted message to a log.
 * @param log The log to add the message to.
 * @param fmt The message (Formatted string).
 * @param ... The arguments list.
 */
void mt_log_add(mt_log_t* log, const char* fmt, ...);

/**
 * @brief Appends a log using a variable argument list.
 * @param log The log to append to.
 * @param fmt The message (Formatted string).
 * @param args The arguments list.
 */
void mt_log_append_v(mt_log_t* log, const char* fmt, va_list args);

/**
 * @brief Appends a log.
 * @param log The log to append to.
 * @param fmt The message (Formatted string).
 * @param args The arguments list.
 */
void mt_log_append(mt_log_t* log, const char* fmt, ...);

/**
 * @brief Dumps the log to the file.
 * @param log The log to dump.
 */
void mt_log_dump(mt_log_t* log);

#define MT_ANSI_CLR "\033[0m"

#define MT_BOLD "\033[1m"
#define MT_RED "\033[31m"
#define MT_GREEN "\033[32m"
#define MT_YELLOW "\033[33m"
#define MT_BLUE "\033[34m"
#define MT_MAGENTA "\033[35m"
#define MT_CYAN "\033[36m"
#define MT_WHITE "\033[37m"

#define MT_VERBOSE \
  (MT_BOLD MT_MAGENTA "Verbose: " MT_ANSI_CLR)
#define MT_WARNING \
  (MT_BOLD MT_YELLOW "Warning: " MT_ANSI_CLR)
#define MT_ERROR \
  (MT_BOLD MT_RED "Error: " MT_ANSI_CLR)

#endif // __MT_LOG_H__
