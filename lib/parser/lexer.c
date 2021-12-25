#include <mutiny/mutiny.h>
#include <mutiny/parser/lexer.h>
#include <mutiny/parser/token.h>
#include <mutiny/settings.h>
#include <mutiny/error/error.h>
#include <mutiny/util/filesystem.h>

#define START(x, y) (!strncmp((x), (y), strlen((y))))
#define END(x, y)   (!strncmp((x) + strlen((x)) - strlen((y)) - 1, (y), strlen((y))))

#define INIT_TOKEN(t, f) \
  do {                                \
    t = malloc(sizeof(mt_token_t));   \
    memset(t, 0, sizeof(mt_token_t)); \
    t->line = f->cur_line;            \
    t->col = f->cur_col;              \
    t->fpos = f->cur_pos;             \
  } while(0)

// Increments the file by one character.
static char next(mt_file_t* file);

// Increments the file by a given number of characters.
static void next_n(mt_file_t* file, size_t n);

// Checks if the current character sequence is a newline.
static bool is_newline(mt_file_t* file);

// Reads the next token in the file.
static mt_token_t* next_token(mt_file_t* file, mt_settings_t* settings);

mt_token_t* tokenize(mt_file_t* f, mt_settings_t* s) {
  mt_token_t* toks = NULL;
  
  mt_token_t* t = NULL;
  while ((t = next_token(f, s))) {
    if (!toks) {
      toks = t;
    }
    else {
      toks->next = t;
    }
    t->first = toks;
    
    if (t->kind == TK_EOF || s->exit_code) {
      break;
    }
  }
  if (!toks) {
    // TODO Warning. "empty file"
  }
  return toks;
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

static const char* kw_strs[]; // TODO Take out when done debugging lexer.

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
      next(f);
      continue;
    }
  }
  
  if (t) {
    printf("[%ld-%ld_%ld][%ld] tok: %d, ", t->line, t->col, t->len, t->fpos, t->kind);
    switch (t->kind) {
      case TK_IDENTIFIER:
        printf("%s\n", t->strval);
        break;
      case TK_KEYWORD:
        printf("%s\n", kw_strs[t->ival]);
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
  mt_token_t* t;
  INIT_TOKEN(t, f);
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
  mt_token_t* t;
  INIT_TOKEN(t, f);
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
    for (c = next(f); c && (isdigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')); c = next(f)) {
      ++t->len;
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
    }
    t->kind = TK_INTEGER;
    val = strndup(first, t->len);
    t->ival = strtoll(val, NULL, 8);
    free(val);
  }
  
  return t;
}

static mt_token_t* read_string_literal(mt_file_t* f, mt_settings_t* s) {
  mt_token_t* t;
  INIT_TOKEN(t, f);
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
  mt_token_t* t;
  INIT_TOKEN(t, f);
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
      c == ';' || c == '?' || c == '(' || c == ')' ||
      c == '[' || c == ']' || c == '{' || c == '}' ||
      START(f->ptr, "!=")) {
    return true;
  }
  return false;
}

static mt_token_t* read_punctuator(mt_file_t* f, mt_settings_t* s) {
  mt_token_t* t;
  INIT_TOKEN(t, f);
  t->kind = TK_PUNCTUATOR;
  t->len = 1;
  
  char c = *f->ptr;
  char c1 = *(f->ptr + 1);
  char c2 = *(f->ptr + 2);
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
  mt_token_t* t;
  INIT_TOKEN(t, f);
  t->kind = TK_EOF;
  return t;
}

static const char* kw_strs[] = {
  [KW_NOT]      = "not",
  [KW_AND]      = "and",
  [KW_OR]       = "or",
  
  [KW_ENUM]     = "enum",
  [KW_STRUCT]   = "struct",
  [KW_UNION]    = "union",
  [KW_FN]       = "fn",
  
  [KW_IF]       = "if",
  [KW_ELIF]     = "elif",
  [KW_ELSE]     = "else",
  [KW_WHILE]    = "while",
  [KW_FOR]      = "for",
  [KW_SWITCH]   = "switch",
  [KW_CASE]     = "case",
  [KW_DEFAULT]  = "default",
  [KW_GOTO]     = "goto",
  
  [KW_RETURN]   = "return",
  [KW_BREAK]    = "break",
  [KW_CONTINUE] = "continue",
  
  [KW_AUTO]     = "auto",
  [KW_REGISTER] = "register",
  [KW_STATIC]   = "static",
  
  [KW_CONST]    = "const",
  [KW_VOLATILE] = "volatile",
  
  [KW_INLINE]   = "inline",
  
  [KW_SIGNED]   = "signed",
  [KW_UNSIGNED] = "unsigned",
};

static void read_keyword(mt_token_t* t) {
  for (size_t i = 0; i < sizeof(kw_strs) / sizeof(char*); ++i) {
    if (!strncmp(t->strval, kw_strs[i], t->len)) {
      t->kind = TK_KEYWORD;
      t->ival = i;
      free(t->strval);
      return;
    }
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
