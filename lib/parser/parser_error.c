#include <mutiny/parser/parser_error.h>
#include <mutiny/util/log.h>
#include <mutiny/util/filesystem.h>
#include <mutiny/mutiny.h>

void mt_log_syntax_error(struct _mt_log* log, struct _mt_file* file, size_t line, size_t col, size_t len, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  
  mt_log_add(log, MT_BOLD "%s:%ld-%ld: " "Invalid Syntax: ", file->path, line, col);
 
  mt_log_append_v(log, fmt, args);
  
  mt_log_append(log, MT_ANSI_CLR "\n");
  
  if (!len) goto END;
  
  char* file_ptr = file->contents;
  
  for (size_t n = 1; n < line; n += (*(file_ptr++) == '\n'));
  
  while (*file_ptr == ' ') {
    file_ptr++;
    col--;
  }
  
  size_t line_len = 0;
  for (line_len = 0; *(file_ptr + line_len) != '\n'; line_len++);
  
  mt_log_append(log, "%.*s", col - 1, file_ptr);

  long int y = (long int)line_len - col - len + 1;
  if (y < 0) {
    len = line_len - col + 1;
  }
  
  mt_log_append(log, MT_BOLD "%.*s" MT_ANSI_CLR, len, file_ptr + col - 1);
  
  mt_log_append(log, "%.*s\n", line_len - col - len + 1, file_ptr + col + len - 1);
  
  for (size_t i = 0; i < col - 1; i++) {
    mt_log_append(log, " ");
  }
  mt_log_append(log, MT_BOLD MT_GREEN "^" MT_ANSI_CLR);
  
  if (len > 1) {
    for (size_t i = 0; i < len - 1; i++) {
      mt_log_append(log, MT_BOLD MT_GREEN "~" MT_ANSI_CLR);
    }
  }
  
  mt_log_append(log, "\n");
  
END:
  va_end(args);
}
