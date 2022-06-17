#include <mutiny/translation_unit/parser/parser_util.h>
#include <mutiny/translation_unit/lexer/token.h>
#include <mutiny/error/error_reporter.h>
#include <mutiny/error/syntax_error.h>
#include <mutiny/mutiny.h>

bool mt_tok_match(mt_token_t* t, mt_token_kind_t k, mt_error_reporter_t* e) {
  if (mt_tok_comp(t, k)) {
    return true;
  }
  
  mt_report_syntax_error(e, t->file, t->line, t->col, t->len, "Unexpected %s, expected %s", mt_token_kind_to_str(t->kind), mt_token_kind_to_str(k));
  
  return false;
}

bool mt_tok_kw_match(mt_token_t* t, mt_keyword_t k, mt_error_reporter_t* e) {
  if (mt_tok_kw_comp(t, k)) {
    return true;
  }
  
  if (mt_tok_comp(t, TK_KEYWORD)) {
    mt_report_syntax_error(e, t->file, t->line, t->col, t->len, "Unexpected %s, expected %s", mt_keyword_to_str(t->kw_val), mt_keyword_to_str(k));
  }
  else {
    mt_report_syntax_error(e, t->file, t->line, t->col, t->len, "Unexpected %s, expected keyword", mt_token_kind_to_str(t->kind));
  }
  return false;

}

bool mt_tok_punct_match(mt_token_t* t, mt_punctuator_t p, mt_error_reporter_t* e) {
  if (mt_tok_punct_comp(t, p)) {
    return true;
  }
  if (mt_tok_comp(t, TK_PUNCTUATOR)) {
    mt_report_syntax_error(e, t->file, t->line, t->col, t->len, "Unexpected `%.*s', expected `%s'", t->len, mt_punct_to_str(t->punct_val), mt_punct_to_str(p));
  }
  else {
    mt_report_syntax_error(e, t->file, t->line, t->col, t->len, "Unexpected %s, expected punctuator", mt_token_kind_to_str(t->kind));
  }
  return false;
}