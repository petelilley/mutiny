#include <mutiny/translation_unit/lexer/lexer.h>
#include <mutiny/translation_unit/lexer/token.h>
#include <mutiny/error/error_reporter.h>
#include <mutiny/error/syntax_error.h>
#include <mutiny/util/filesystem.h>
#include <mutiny/util/log.h>

#include <mutiny/translation_unit/lexer/char_literal.h>

mt_token_t* mt_tokenize_char_literal(mt_file_t* f, mt_error_reporter_t* e) {
  mt_token_t* t = mt_token_init(f);
  t->kind = TK_CHAR;
  
  ++t->col;
  ++t->fpos;
  
  char* first = f->ptr + 1;
  
  size_t line = f->cur_line, col = f->cur_col;
  
  char c;
  for (c = mt_file_getc(f); c && c != '\''; c = mt_file_getc(f)) {
    ++t->len;
  }
  
  if (!c) {
    mt_report_syntax_error(e, f, line, col, 1, "Unterminated character literal");
    return t;
  }
  
  mt_file_getc(f);
  
  if (t->len < 1) {
    mt_report_syntax_error(e, f, line, col, 2, "Character constant empty");
    return t;
  }
  else if (t->len > 1) {
    mt_report_syntax_error(e, f, line, col, 1, "Character constant too long");
    return t;
  }
  
  t->c_val = *first;
  
  return t;
}