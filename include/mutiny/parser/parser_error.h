#ifndef __MT_PARSER_ERROR_H__
#define __MT_PARSER_ERROR_H__

#include <mutiny/mutiny.h>

struct _mt_log;
struct _mt_file;

void mt_log_syntax_error(struct _mt_log* log, struct _mt_file* file, size_t line, size_t col, size_t len, const char* fmt, ...);

#endif // __MT_PARSER_ERROR_H__
