#include <mutiny/error/syntax_error.h>
#include <mutiny/error/syntax_message.h>
#include <mutiny/error/error_reporter.h>
#include <mutiny/util/log.h>
#include <mutiny/util/filesystem.h>
#include <mutiny/mutiny.h>

void mt_report_syntax_error(mt_error_reporter_t* err, mt_file_t* file, size_t line, size_t col, size_t len, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  
  // Error Message.
  mt_log_append(&err->err_log, MT_BOLD "%s:%ld-%ld: " MT_ANSI_CLR, file->path, line, col);
  mt_log_add(&err->err_log, MT_BOLD "Invalid Syntax: ");
  mt_log_append_v(&err->err_log, fmt, args);
  mt_log_append(&err->err_log, MT_ANSI_CLR "\n");
  
  if (len) {
    mt_report_syntax(&err->err_log, file, line, col, len, NULL);
  }
  
  // Increment the error count.
  err->err_num++;
  
  va_end(args);
}