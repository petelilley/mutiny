#include <mutiny/translation_unit/lexer/lexer.h>
#include <mutiny/translation_unit/lexer/token.h>
#include <mutiny/translation_unit/parser/parser_util.h>
#include <mutiny/error/syntax_error.h>
#include <mutiny/util/filesystem.h>
#include <mutiny/util/log.h>

#include <mutiny/translation_unit/lexer/punctuator.h>

bool mt_is_punctuator(const mt_file_t* f) {
  char c = *f->ptr;
  if (c == '+' || c == '-' || c == '*' || c == '/' ||
      c == '%' || c == '~' || c == '&' || c == '|' ||
      c == '^' || c == '<' || c == '>' || c == '=' ||
      c == '#' || c == '.' || c == ',' || c == ':' ||
      c == ';' || c == '?' || c == '@' ||
      c == '(' || c == ')' || c == '[' || c == ']' ||
      c == '{' || c == '}' ||
      MT_STR_START(f->ptr, "!=")) {
    return true;
  }
  return false;
}

mt_token_t* mt_tokenize_punctuator(mt_file_t* f) {
  mt_token_t* t = mt_token_init(f);
  t->kind = TK_PUNCTUATOR;
  t->len = 1;
  
  char c = *f->ptr;
  char c1 = *(f->ptr + 1);
  switch (c) {
    case '+':
    case '-':
      if (c1 == c || (c == '-' && c1 == '>')) { // ++ or -- or ->
        t->len = 2;
        break;
      }
      // fall through
    case '*':
    case '/':
    case '%':
    case '&':
    case '|':
    case '^':
      if (c1 == '=') { // += or -= or *= or /= or %= or &= or |= or ^=
        t->len = 2;
      }
      break;
    case '<':
    case '>':
      if (c1 == c) { // << or >>
        char c2 = *(f->ptr + 2);
        
        t->len = 2;
        if (c2 == '=') { // <<= or >>=
          t->len = 3;
        }
      }
      else if (c1 == '=') { // <= or >=
        t->len = 2;
      }
      break;
    case '=':
      if (c1 == '=') { // ==
        t->len = 2;
      }
      break;
    case '!': // !=
      t->len = 2;
      break;
    case ':':
      if (c1 == '=' || c1 == ':') { // := or ::
        t->len = 2;
      }
      break;
  }
  
  t->punct_val = mt_str_to_punct(f->ptr, t->len);
  
  mt_file_skip(f, t->len);
  
  return t;
}