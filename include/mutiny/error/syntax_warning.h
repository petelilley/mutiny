#ifndef __MT_SYNTAX_WARNING_H__
#define __MT_SYNTAX_WARNING_H__

#include <mutiny/mutiny.h>

struct _mt_error_reporter;
struct _mt_file;

/**
 * @brief Reports a syntax warning.
 * @param error_reporter The translation unit's error reporter.
 * @param file The file where the warning occurred.
 * @param line The line where the warning occurred.
 * @param column The column where the warning occurred.
 * @param len The length of the token concerned.
 * @param fmt The warning message (Formatted string).
 */
void mt_report_syntax_warning(struct _mt_error_reporter* error_reporter, struct _mt_file* file, size_t line, size_t col, size_t len, const char* fmt, ...);

#endif // __MT_SYNTAX_WARNING_H__