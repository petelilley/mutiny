#include <mutiny/translation_unit/lexer/lexer.h>
#include <mutiny/translation_unit/lexer/token.h>
#include <mutiny/error/error_reporter.h>
#include <mutiny/error/syntax_error.h>
#include <mutiny/util/filesystem.h>
#include <mutiny/util/log.h>

#include <mutiny/translation_unit/lexer/identifier.h>

static void mt_tokenize_keyword(mt_token_t* t) {
  int kw = mt_str_to_keyword(t->strval, t->len);
  if (kw != -1) {
      t->kind = TK_KEYWORD;
      t->ival = kw;
      free(t->strval);
      return;
  }
}

mt_token_t* mt_tokenize_identifier(mt_file_t* f) {
  mt_token_t* t = mt_token_init(f);
  t->kind = TK_IDENTIFIER;
  t->len = 1;
  
  char* first = f->ptr;
  
  for (char c = mt_file_getc(f); c && (isalpha(c) || c == '_' || isdigit(c)); c = mt_file_getc(f)) {
    ++t->len;
  }
  
  t->strval = strndup(first, t->len);
  
  mt_tokenize_keyword(t);
  
  return t;
}