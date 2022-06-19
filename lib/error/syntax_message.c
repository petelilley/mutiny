#include <mutiny/error/syntax_message.h>
#include <mutiny/error/error_reporter.h>
#include <mutiny/util/log.h>
#include <mutiny/util/filesystem.h>
#include <mutiny/mutiny.h>

void mt_report_syntax(mt_log_t* log, mt_file_t* file, size_t line, size_t col, size_t len, const char* correction) {
  char* file_ptr = file->contents;
  
  // Advance to the start of the current line.
  for (size_t n = 1; n < line; n += (*(file_ptr++) == '\n'));
  
  // Skip over leading whitespace.
  while (*file_ptr == ' ') {
    file_ptr++;
    col--;
  }
  
  // Count the length of the line.
  size_t line_len = 0;
  for (line_len = 0; *(file_ptr + line_len) != '\n'; line_len++);
  
  char line_str[32] = "";
  sprintf(line_str, "%zu", line);
  size_t line_str_len = strlen(line_str);
  
  size_t pre_len = line_str_len;
  
  if (pre_len < 6) {
    pre_len = 6;
  }
  
  pre_len += 5;
  
  char pre[pre_len];
  memset(pre, ' ', pre_len);
  pre[pre_len - 1] = '\0';
  pre[pre_len - 4] = '|';
  
  char pre1[pre_len];
  memcpy(pre1, pre, pre_len);
  
  memcpy(pre1 + pre_len - 5 - line_str_len, line_str, line_str_len);
  
  // Display the beginning of the line before the error token.
  mt_log_append(log, "%.*s%.*s", pre_len, pre1, col - 1, file_ptr);

  // Adjust the length of the error token based on the line length.
  long int y = (long int)line_len - col - len + 1;
  if (y < 0) {
    len = line_len - col + 1;
  }
  
  // Display the error token.
  mt_log_append(log, MT_BOLD "%.*s" MT_ANSI_CLR, len, file_ptr + col - 1);
  // Display the rest of the line.
  mt_log_append(log, "%.*s\n", line_len - col - len + 1, file_ptr + col + len - 1);

  char ws[col - 1];
  memset(ws, ' ', sizeof(ws));
  ws[sizeof(ws)] = '\0';
  
  mt_log_append(log, "%.*s%s", pre_len, pre, ws);
  
  // Display an arrow pointing to the error token.
  mt_log_append(log, MT_BOLD MT_GREEN "^" MT_ANSI_CLR);
  
  // Display markers showing extent of error token.
  if (len > 1) {
    for (size_t i = 0; i < len - 1; i++) {
      mt_log_append(log, MT_BOLD MT_GREEN "~" MT_ANSI_CLR);
    }
  }
  
  // End the line.
  mt_log_append(log, "\n");
  
  if (!correction) return;
  
  mt_log_append(log, "%.*s%s" MT_GREEN MT_BOLD "|\n" MT_ANSI_CLR, pre_len, pre, ws);
  mt_log_append(log, "%.*s%s" MT_GREEN MT_BOLD "%s\n" MT_ANSI_CLR, pre_len, pre, ws, correction);
}