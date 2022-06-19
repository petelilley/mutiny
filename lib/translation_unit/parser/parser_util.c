#include <mutiny/translation_unit/parser/parser_util.h>
#include <mutiny/translation_unit/lexer/token.h>
#include <mutiny/error/error_reporter.h>
#include <mutiny/error/syntax_error.h>
#include <mutiny/mutiny.h>

#define MAX_EXPECTED_TOKENS 3

mt_token_kind_t mt_token_match(mt_error_reporter_t* e, mt_token_t* t, size_t n, ...) {
  if (!n) return TK_UNKNOWN;
  va_list args;
  va_start(args, n);
  
  mt_token_kind_t kinds[n];
  for (size_t i = 0; i < n; i++) {
    kinds[i] = va_arg(args, mt_token_kind_t);
  }
  
  va_end(args);
  
  for (size_t i = 0; i < n; i++) {
    mt_token_kind_t k = kinds[i];
    if (MT_TOK_COMP(t, k)) {
      return k;
    }
  }
  
  char expected[256] = "";
  
  if (n <= MAX_EXPECTED_TOKENS) {
    strcat(expected, ", expected ");
    
    for (size_t i = 0; i < n; i++) {
      mt_token_kind_t k = kinds[i];
      strcat(expected, mt_token_kind_to_str(k));
      if (i < n - 1) strcat(expected, " or ");
    }
  }
  
  mt_report_syntax_error(e, t->file, t->line, t->col, t->len, "Unexpected %s%s", mt_token_kind_to_str(t->kind), expected);
  
  return TK_UNKNOWN;
}

mt_keyword_t mt_token_match_kw(mt_error_reporter_t* e, mt_token_t* t, size_t n, ...) {
  if (!n) return KW_UNKNOWN;
  va_list args;
  va_start(args, n);
  
  mt_keyword_t kws[n];
  for (size_t i = 0; i < n; i++) {
    kws[i] = va_arg(args, mt_keyword_t);
  }
  
  va_end(args);
  
  for (size_t i = 0; i < n; i++) {
    mt_keyword_t k = kws[i];
    if (MT_TOK_COMP_KW(t, k)) {
      return k;
    }
  }
  
  if (MT_TOK_COMP(t, TK_KEYWORD)) {
    char expected[256] = "";
    
    if (n <= MAX_EXPECTED_TOKENS) {
      strcat(expected, ", expected ");
      
      for (size_t i = 0; i < n; i++) {
        mt_keyword_t k = kws[i];
        strcat(expected, mt_keyword_to_str(k));
        if (i < n - 1) strcat(expected, " or ");
      }
    }
    
    mt_report_syntax_error(e, t->file, t->line, t->col, t->len, "Unexpected %s%s", mt_keyword_to_str(t->kw_val), expected);
  }
  else {
    mt_report_syntax_error(e, t->file, t->line, t->col, t->len, "Unexpected %s, expected keyword", mt_token_kind_to_str(t->kind));
  }
  
  return KW_UNKNOWN;
}

mt_punctuator_t mt_token_match_punct(mt_error_reporter_t* e, mt_token_t* t, size_t n, ...) {
  if (!n) return PCT_UNKNOWN;
  va_list args;
  va_start(args, n);
  
  mt_punctuator_t puncts[n];
  for (size_t i = 0; i < n; i++) {
    puncts[i] = va_arg(args, mt_punctuator_t);
  }
  
  va_end(args);
  
  for (size_t i = 0; i < n; i++) {
    mt_punctuator_t p = puncts[i];
    if (MT_TOK_COMP_PUNCT(t, p)) {
      return p;
    }
  }
  
  if (MT_TOK_COMP(t, TK_PUNCTUATOR)) {
    char expected[256] = "";
    
    if (n <= MAX_EXPECTED_TOKENS) {
      strcat(expected, ", expected ");
      
      for (size_t i = 0; i < n; i++) {
        mt_punctuator_t p = puncts[i];
        sprintf(expected + strlen(expected), "`%s'", mt_punct_to_str(p));
        if (i < n - 1) strcat(expected, " or ");
      }
    }
    
    mt_report_syntax_error(e, t->file, t->line, t->col, t->len, "Unexpected `%s'%s", mt_punct_to_str(t->punct_val), expected);
  }
  else {
    mt_report_syntax_error(e, t->file, t->line, t->col, t->len, "Unexpected %s, expected punctuator", mt_token_kind_to_str(t->kind));
  }
  
  return PCT_UNKNOWN;
}