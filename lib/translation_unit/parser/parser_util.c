#include <mutiny/translation_unit/parser/parser_util.h>
#include <mutiny/translation_unit/lexer/token.h>
#include <mutiny/error/syntax_error.h>
#include <mutiny/mutiny.h>

bool mt_tok_match(mt_token_t* t, mt_token_kind_t k) {
  if (mt_tok_comp(t, k)) {
    return true;
  }
  // TODO: Syntax error.
  printf("error D:\n");
  return false;
}

bool mt_tok_kw_match(mt_token_t* t, mt_keyword_t k) {
  if (mt_tok_kw_comp(t, k)) {
    return true;
  }
  // TODO: Syntax error.
  printf("error D:\n");
  return false;

}

bool mt_tok_punct_match(mt_token_t* t, const char* p) {
  if (mt_tok_punct_comp(t, p)) {
    return true;
  }
  // TODO: Syntax error.
  printf("error D:\n");
  return false;
}