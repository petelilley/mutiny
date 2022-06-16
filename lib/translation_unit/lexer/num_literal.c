#include <mutiny/translation_unit/lexer/lexer.h>
#include <mutiny/translation_unit/lexer/token.h>
#include <mutiny/error/error_reporter.h>
#include <mutiny/error/syntax_error.h>
#include <mutiny/util/filesystem.h>
#include <mutiny/util/log.h>

#include <mutiny/translation_unit/lexer/num_literal.h>

mt_token_t* mt_tokenize_numeric_literal(mt_file_t* f, mt_error_reporter_t* e) {
  mt_token_t* t = mt_token_init(f);
  t->len = 1;
  
  char c = *f->ptr;
  char* first = f->ptr;
  char* val;
  
  // Decimal number.
  if (c - '0' > 0 || c == '.') {
    bool fp = c == '.';
    
    for (c = mt_file_getc(f); c && isdigit(c); c = mt_file_getc(f)) {
      ++t->len;
    }
    
    if (!fp && c == '.') {
      fp = true;
      ++t->len;
      for (c = mt_file_getc(f); c && isdigit(c); c = mt_file_getc(f)) {
        ++t->len;
      }
    }
    val = strndup(first, t->len);
    if (fp) {
      t->kind = TK_FLOAT;
      t->fval = atof(val);
    }
    else {
      t->kind = TK_INTEGER;
      t->ival = atoll(val);
    }
    free(val);
  }
  // Hexadecimal number.
  else if (*(f->ptr + 1) == 'x' || *(f->ptr + 1) == 'X') {
    mt_file_getc(f);
    first += 2;
    
    c = mt_file_getc(f);
    while (c && (isdigit(c) || isalpha(c))) {
      ++t->len;
      
      if ((c > 'f' && c <= 'z') || (c > 'F' && c <= 'Z')) {
        mt_report_syntax_error(e, f, f->cur_line, f->cur_col, 1, "Invalid suffix `x%c' on integer literal", c);
      }
      c = mt_file_getc(f);
    }
    if (t->len == 1) {
      mt_report_syntax_error(e, f, f->cur_line, f->cur_col - 1, 1, "Invalid suffix 'x' on integer literal");
    }
    
    t->kind = TK_INTEGER;
    val = strndup(first, t->len);
    t->ival = strtoll(val, NULL, 16);
    free(val);
  }
  // Octal number.
  else {
    for (c = mt_file_getc(f); c && isdigit(c); c = mt_file_getc(f)) {
      ++t->len;
      if (c - '0' > 7) {
        mt_report_syntax_error(e, f, f->cur_line, f->cur_col, 1, "Invalid digit '%d' in octal numeric literal", c - '0');
      }
    }
    t->kind = TK_INTEGER;
    val = strndup(first, t->len);
    t->ival = strtoll(val, NULL, 8);
    free(val);
  }
  
  return t;
}