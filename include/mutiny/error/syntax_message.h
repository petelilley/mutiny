#ifndef __MT_SYNTAX_MESSAGE_H__
#define __MT_SYNTAX_MESSAGE_H__

#include <mutiny/mutiny.h>

struct _mt_log;
struct _mt_file;

/**
 * @brief Reports specified syntax.
 * @param log The log to report to.
 * @param file The file where the token lies.
 * @param line The line where the token lies.
 * @param column The column where the token lies.
 * @param len The length of the token concerned.
 * @param correction The correction to be presented. NULL if no correction is available.
 */
void mt_report_syntax(struct _mt_log* log, struct _mt_file* file, size_t line, size_t col, size_t len, const char* correction);

#endif // __MT_SYNTAX_MESSAGE_H__