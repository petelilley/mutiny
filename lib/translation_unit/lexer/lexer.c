#include <mutiny/mutiny.h>
#include <mutiny/translation_unit/lexer/lexer.h>
#include <mutiny/translation_unit/lexer/token.h>
#include <mutiny/translation_unit/translation_unit.h>
#include <mutiny/translation_unit/parser/parser_util.h>
#include <mutiny/error/syntax_error.h>
#include <mutiny/error/syntax_warning.h>
#include <mutiny/settings.h>
#include <mutiny/util/filesystem.h>
#include <mutiny/util/log.h>

#include <mutiny/translation_unit/lexer/char_literal.h>
#include <mutiny/translation_unit/lexer/str_literal.h>
#include <mutiny/translation_unit/lexer/num_literal.h>
#include <mutiny/translation_unit/lexer/identifier.h>
#include <mutiny/translation_unit/lexer/punctuator.h>
#include <mutiny/translation_unit/lexer/comment.h>

// Reads the next token in the file.
static mt_token_t* next_token(mt_file_t* file, mt_error_reporter_t* error_reporter);

void mt_translation_unit_tokenize(mt_translation_unit_t* t_unit) {
  mt_token_t* first = NULL;
  mt_token_t* head = NULL;
  
  mt_token_t* t = NULL;
  while ((t = next_token(t_unit->file, &t_unit->error_reporter))) {
    if (!first) {
      first = t;
      head = t;
    }
    else {
      t->first = first;
      head->next = t;
      head = t;
    }
    
    if (head->kind == TK_EOF || t_unit->settings->exit_code) {
      break;
    }
  }
  if (!first || first->kind == TK_EOF) {
    mt_report_syntax_warning(&t_unit->error_reporter, t_unit->file, 0, 0, 0, "Translation unit is empty");
  }
  t_unit->tokens = first;
}

static mt_token_t* next_token(mt_file_t* f, mt_error_reporter_t* e) {
  mt_token_t* t = NULL;
  
  char c;
  while (f->ptr) {
    c = *f->ptr;
    
    if (isspace(c)) {
      mt_file_getc(f);
      continue;
    }
    else if (MT_STR_START(f->ptr, "//")) {
      mt_skip_line_comment(f);
      continue;
    }
    else if (MT_STR_START(f->ptr, "/*")) {
      mt_file_skip(f, 2);
      mt_skip_block_comment(f, e);
      continue;
    }
    else if (isalpha(c) || c == '_') {
      t = mt_tokenize_identifier(f);
      break;
    }
    else if (isdigit(c) || (c == '.' && isdigit(*(f->ptr + 1)))) {
      t = mt_tokenize_numeric_literal(f, e);
      break;
    }
    else if (c == '"') {
      t = mt_tokenize_string_literal(f, e);
      break;
    }
    else if (c == '\'') {
      t = mt_tokenize_char_literal(f, e);
      break;
    }
    else if (mt_is_punctuator(f)) {
      t = mt_tokenize_punctuator(f);
      break;
    }
    else if (!c) {
      t = mt_token_init(f);
      t->kind = TK_EOF;
      break;
    }
    else {
      mt_report_syntax_error(e, f, f->cur_line, f->cur_col, 1, "Invalid token `%c'", c);
      mt_file_getc(f);
      continue;
    }
  }
  
#if 1
  if (t) {
    printf("[%ld-%ld_%ld][%ld] tok: ", t->line, t->col, t->len, t->fpos);
    switch (t->kind) {
      case TK_UNKNOWN:
        printf("UNKNOWN");
        break;
      case TK_IDENTIFIER:
        printf("id, \"%s\"\n", t->str_val);
        break;
      case TK_KEYWORD:
        printf("kw, %s\n", mt_keyword_to_str(t->kw_val));
        break;
      case TK_PUNCTUATOR:
        printf("pct, '%s'\n", mt_punct_to_str(t->punct_val));
        break;
      case TK_INTEGER:
        printf("int, %lld\n", t->i_val);
        break;
      case TK_FLOAT:
        printf("fl, %Lf\n", t->f_val);
        break;
      case TK_STRING:
        printf("str, \"%s\"\n", t->str_val);
        break;
      case TK_CHAR:
        printf("chr, '%c'\n", t->c_val);
        break;
      case TK_EOF:
        printf("EOF\n");
        break;
    }
  }
#endif
  
  return t;
}