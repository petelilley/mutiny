#include <mutiny/error/syntax_warning.h>
#include <mutiny/error/syntax_message.h>
#include <mutiny/error/error_reporter.h>
#include <mutiny/util/log.h>
#include <mutiny/util/filesystem.h>
#include <mutiny/mutiny.h>

void mt_report_syntax_warning(mt_error_reporter_t* err, mt_file_t* file, size_t line, size_t col, size_t len, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  
  // Warning Message.
  mt_log_append(&err->warn_log, MT_BOLD "%s:%ld-%ld: " MT_ANSI_CLR, file->path, line, col);
  mt_log_add(&err->warn_log, MT_BOLD);
  mt_log_append_v(&err->warn_log, fmt, args);
  mt_log_append(&err->warn_log, MT_ANSI_CLR "\n");
  
  if (len) {
    mt_report_syntax(&err->warn_log, file, line, col, len, NULL);
  }
  
  // Increment the error count.
  err->warn_num++;
  
  va_end(args);
}