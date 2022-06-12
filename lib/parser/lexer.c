#include <mutiny/mutiny.h>
#include <mutiny/parser/lexer.h>
#include <mutiny/parser/token.h>
#include <mutiny/parser/translation_unit.h>
#include <mutiny/parser/parser_util.h>
#include <mutiny/settings.h>
#include <mutiny/util/filesystem.h>

// Increments the file by one character.
static char next(mt_file_t* file);

// Increments the file by a given number of characters.
static void next_n(mt_file_t* file, size_t n);

// Checks if the current character sequence is a newline.
static bool is_newline(mt_file_t* file);

// Reads the next token in the file.
static mt_token_t* next_token(mt_file_t* file, mt_settings_t* settings);

bool mt_translation_unit_tokenize(mt_translation_unit_t* t_unit) {
  mt_token_t* first = NULL;
  mt_token_t* head = NULL;
  
  mt_token_t* t = NULL;
  while ((t = next_token(t_unit->file, t_unit->settings))) {
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
  if (!first) {
    // TODO Warning. "empty file"
  }
  t_unit->tokens = first;
  return true;
}

static void skip_line_comment(mt_file_t* file);
static void skip_block_comment(mt_file_t* file);
static mt_token_t* read_identifier(mt_file_t* file);
static mt_token_t* read_numeric_literal(mt_file_t* file, mt_settings_t* settings);
static mt_token_t* read_string_literal(mt_file_t* file, mt_settings_t* settings);
static mt_token_t* read_char_literal(mt_file_t* file, mt_settings_t* settings);
static bool is_punctuator(const mt_file_t* file);
static mt_token_t* read_punctuator(mt_file_t* file, mt_settings_t* settings);
static mt_token_t* read_eof(mt_file_t* file);
static void read_keyword(mt_token_t* token);

static mt_token_t* next_token(mt_file_t* f, mt_settings_t* s) {
  mt_token_t* t = NULL;
  
  char c;
  while (f->ptr) {
    c = *f->ptr;
    
    if (isspace(c)) {
      next(f);
      continue;
    }
    else if (START(f->ptr, "//")) {
      skip_line_comment(f);
      continue;
    }
    else if (START(f->ptr, "/*")) {
      skip_block_comment(f);
      continue;
    }
    else if (isalpha(c) || c == '_') {
      t = read_identifier(f);
      break;
    }
    else if (isdigit(c) || (c == '.' && isdigit(*(f->ptr + 1)))) {
      t = read_numeric_literal(f, s);
      break;
    }
    else if (c == '"') {
      t = read_string_literal(f, s);
      break;
    }
    else if (c == '\'') {
      t = read_char_literal(f, s);
      break;
    }
    else if (is_punctuator(f)) {
      t = read_punctuator(f, s);
      break;
    }
    else if (!c) {
      t = read_eof(f);
      break;
    }
    else {
      // TODO Syntax error. "invalid token"
      puts("invalid token");
      next(f);
      continue;
    }
  }
  
#if 1
  
  if (t) {
    printf("[%ld-%ld_%ld][%ld] tok: %d, ", t->line, t->col, t->len, t->fpos, t->kind);
    switch (t->kind) {
      case TK_IDENTIFIER:
        printf("%s\n", t->strval);
        break;
      case TK_KEYWORD:
        printf("%s\n", mt_keyword_to_str(t->ival));
        break;
      case TK_PUNCTUATOR:
        printf("%s\n", t->strval);
        break;
      case TK_INTEGER:
        printf("%lld\n", t->ival);
        break;
      case TK_FLOAT:
        printf("%Lf\n", t->fval);
        break;
      case TK_STRING:
        printf("%s\n", t->strval);
        break;
      case TK_CHAR:
        printf("%c\n", t->cval);
        break;
      case TK_EOF:
        printf("EOF\n");
        break;
    }
  }

#endif
  
  return t;
}

static void skip_line_comment(mt_file_t* f) {
  char c;
  while ((c = next(f)) && (c != '\n' && c != '\r'));
}

static void skip_block_comment(mt_file_t* f) {
  next_n(f, 2);
  for (char c = *f->ptr; c; c = next(f)) {
    if (c == '/') {
      if (*(f->ptr + 1) == '*') {
        next(f);
        // Nested block comments. What's so hard about this C and C++????
        skip_block_comment(f);
      }
    }
    else if (c == '*') {
      if (*(f->ptr + 1) == '/') {
        next(f);
        break;
      }
    }
  }
  next(f);
}

static mt_token_t* read_identifier(mt_file_t* f) {
  mt_token_t* t = mt_token_init(f);
  t->kind = TK_IDENTIFIER;
  t->len = 1;
  
  char* first = f->ptr;
  
  for (char c = next(f); c && (isalpha(c) || c == '_' || isdigit(c)); c = next(f)) {
    ++t->len;
  }
  
  t->strval = strndup(first, t->len);
  
  read_keyword(t);
  
  return t;
}

static mt_token_t* read_numeric_literal(mt_file_t* f, mt_settings_t* s) {
  mt_token_t* t = mt_token_init(f);
  t->len = 1;
  
  char c = *f->ptr;
  char* first = f->ptr;
  char* val;
  
  // Decimal number.
  if (c - '0' > 0 || c == '.') {
    bool fp = c == '.';
    
    for (c = next(f); c && isdigit(c); c = next(f)) {
      ++t->len;
    }
    
    if (!fp && c == '.') {
      fp = true;
      ++t->len;
      for (c = next(f); c && isdigit(c); c = next(f)) {
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
    next(f);
    first += 2;
    for (c = next(f); c && (isdigit(c) || isalpha(c)); c = next(f)) {
      if ((c > 'f' && c <= 'z') || (c > 'F' && c <= 'Z')) {
        // TODO Syntax error. "Invalid suffix 'xG' on integer literal"
        printf("invalid suffix 'x%c' on integer literal\n", c);
        break;
      }
      ++t->len;
    }
    if (t->len == 1) {
        puts("invalid suffix 'x' on integer literal");
      // TODO Syntax error. "Invalid suffix 'x' on integer literal"
    }
    t->kind = TK_INTEGER;
    val = strndup(first, t->len);
    t->ival = strtoll(val, NULL, 16);
    free(val);
  }
  // Octal number.
  else {
    for (c = next(f); c && isdigit(c); c = next(f)) {
      ++t->len;
      if (c - '0' > 7) {
        printf("invalid digit '%d' in octal numeric literal\n", c - '0');
        // TODO Syntax error. "Invalid digit '8' in octal numeric literal"
      }
    }
    t->kind = TK_INTEGER;
    val = strndup(first, t->len);
    t->ival = strtoll(val, NULL, 8);
    free(val);
  }
  
  return t;
}

static mt_token_t* read_string_literal(mt_file_t* f, mt_settings_t* s) {
  mt_token_t* t = mt_token_init(f);
  t->kind = TK_STRING;
  
  ++t->col;
  ++t->fpos;
  
  char* first = f->ptr + 1;
  
  char c;
  for (c = next(f); c && c != '"'; c = next(f)) {
    ++t->len;
  }
  
  if (!c) {
    // TODO Syntax error. "Unterminated string literal"
    t->kind = TK_EOF;
    return t;
  }
  next(f);
  
  t->strval = strndup(first, t->len);
  
  return t;
}

static mt_token_t* read_char_literal(mt_file_t* f, mt_settings_t* s) {
  mt_token_t* t = mt_token_init(f);
  t->kind = TK_CHAR;
  
  ++t->col;
  ++t->fpos;
  
  char* first = f->ptr + 1;
  
  char c;
  for (c = next(f); c && c != '\''; c = next(f)) {
    ++t->len;
  }
  
  if (t->len != 1) {
    // TODO Syntax error. "Character constant empty / too long"
    return t;
  }
  
  if (!c) {
    // TODO Syntax error. "Unterminated character literal"
    t->kind = TK_EOF;
    return t;
  }
  next(f);
  
  t->cval = *first;
  
  return t;
}

static bool is_punctuator(const mt_file_t* f) {
  char c = *f->ptr;
  if (c == '+' || c == '-' || c == '*' || c == '/' ||
      c == '%' || c == '~' || c == '&' || c == '|' ||
      c == '^' || c == '<' || c == '>' || c == '=' ||
      c == '#' || c == '.' || c == ',' || c == ':' ||
      c == ';' || c == '?' || c == '@' ||
      c == '(' || c == ')' || c == '[' || c == ']' ||
      c == '{' || c == '}' ||
      START(f->ptr, "!=")) {
    return true;
  }
  return false;
}

static mt_token_t* read_punctuator(mt_file_t* f, mt_settings_t* s) {
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
  
  t->strval = strndup(f->ptr, t->len);
  
  next_n(f, t->len);
  
  return t;
}

static mt_token_t* read_eof(mt_file_t* f) {
  mt_token_t* t = mt_token_init(f);
  t->kind = TK_EOF;
  return t;
}

static void read_keyword(mt_token_t* t) {
  int kw = mt_str_to_keyword(t->strval, t->len);
  if (kw != -1) {
      t->kind = TK_KEYWORD;
      t->ival = kw;
      free(t->strval);
      return;
  }
}

static char next(mt_file_t* f) {
  if (is_newline(f)) {
    ++f->cur_line;
    f->cur_col = 1;
  }
  else {
    ++f->cur_col;
  }
  // TODO Read escape sequences.
  
  ++f->cur_pos;
  return *++f->ptr;
}

static void next_n(mt_file_t* f, size_t n) {
  for (size_t i = 0; i < n; ++i) {
    next(f);
  }
}

static bool is_newline(mt_file_t* f) {
  char c = *f->ptr;
  if (c == '\n') return true;
  if (c != '\r') return false;
  if (*(f->ptr + 1) == '\n') {
    ++f->cur_col;
    ++f->cur_pos;
    ++f->ptr;
  }
  return true;
}
