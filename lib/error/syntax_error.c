#include <mutiny/error/syntax_error.h>
#include <mutiny/error/error_reporter.h>
#include <mutiny/util/log.h>
#include <mutiny/util/filesystem.h>
#include <mutiny/mutiny.h>

void mt_report_syntax_error(mt_error_reporter_t* err, mt_file_t* file, size_t line, size_t col, size_t len, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  
  mt_log_add(&err->err_log, MT_BOLD "%s:%ld-%ld: " "Invalid Syntax: ", file->path, line, col);
 
  mt_log_append_v(&err->err_log, fmt, args);
  
  mt_log_append(&err->err_log, MT_ANSI_CLR "\n");
  
  if (!len) goto END;
  
  char* file_ptr = file->contents;
  
  for (size_t n = 1; n < line; n += (*(file_ptr++) == '\n'));
  
  while (*file_ptr == ' ') {
    file_ptr++;
    col--;
  }
  
  size_t line_len = 0;
  for (line_len = 0; *(file_ptr + line_len) != '\n'; line_len++);
  
  mt_log_append(&err->err_log, "%.*s", col - 1, file_ptr);

  long int y = (long int)line_len - col - len + 1;
  if (y < 0) {
    len = line_len - col + 1;
  }
  
  mt_log_append(&err->err_log, MT_BOLD "%.*s" MT_ANSI_CLR, len, file_ptr + col - 1);
  
  mt_log_append(&err->err_log, "%.*s\n", line_len - col - len + 1, file_ptr + col + len - 1);
  
  for (size_t i = 0; i < col - 1; i++) {
    mt_log_append(&err->err_log, " ");
  }
  mt_log_append(&err->err_log, MT_BOLD MT_GREEN "^" MT_ANSI_CLR);
  
  if (len > 1) {
    for (size_t i = 0; i < len - 1; i++) {
      mt_log_append(&err->err_log, MT_BOLD MT_GREEN "~" MT_ANSI_CLR);
    }
  }
  
  mt_log_append(&err->err_log, "\n");
  
END:
  err->err_num++;
  va_end(args);
}