#ifndef __MT_SYNTAX_ERROR_H__
#define __MT_SYNTAX_ERROR_H__

#include <mutiny/mutiny.h>

struct _mt_log;
struct _mt_file;

/**
 * @brief Logs a syntax error.
 * @param log The log to log the error to.
 * @param file The file where the error occurred.
 * @param line The line where the error occurred.
 * @param column The column where the error occurred.
 * @param len The length of the unexpected token.
 * @param fmt The error message (Formatted string).
 */
void mt_log_syntax_error(struct _mt_log* log, struct _mt_file* file, size_t line, size_t col, size_t len, const char* fmt, ...);

#endif // __MT_SYNTAX_ERROR_H__
