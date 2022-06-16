#include <mutiny/translation_unit/lexer/lexer.h>
#include <mutiny/translation_unit/lexer/token.h>
#include <mutiny/error/error_reporter.h>
#include <mutiny/error/syntax_error.h>
#include <mutiny/util/filesystem.h>
#include <mutiny/util/log.h>

#include <mutiny/translation_unit/lexer/comment.h>

void mt_skip_line_comment(mt_file_t* f) {
  char c;
  while ((c = mt_file_getc(f)) && (c != '\n' && c != '\r'));
}

void mt_skip_block_comment(mt_file_t* f, mt_error_reporter_t* e) {
  char c = *f->ptr;
  
  while (1) {
    if (!c) {
      mt_report_syntax_error(e, f, f->cur_line, f->cur_col, 0, "Unterminated /* Comment");
      break;
    }
    
    if (c == '/' && *(f->ptr + 1) == '*') {
      mt_file_skip(f, 2);
      mt_skip_block_comment(f, e);
      if (!*f->ptr) break;
    }
    if (c == '*' && *(f->ptr + 1) == '/') {
      mt_file_getc(f);
      break;
    }
    
    c = mt_file_getc(f);
  }
}