#include <mutiny/mutiny.h>
#include <mutiny/parser/lexer.h>
#include <mutiny/settings.h>
#include <mutiny/error/error.h>
#include <mutiny/util/filesystem.h>

#define START(x, y) (!strncmp((x), (y), strlen((y))))
#define END(x, y)   (!strncmp((x) + strlen((x)) - strlen((y)) - 1, (y), strlen((y))))

// Increments the file by one character.
static char next(mt_file_t* file);

// Increments the file by a given number of characters.
static void next_n(mt_file_t* file, size_t n);

// Checks if the current character sequence is a newline.
static bool is_newline(mt_file_t* file);

// Returns the next token.
static mt_token_t* next_token(mt_file_t* file, mt_settings_t* settings);

mt_token_t* tokenize(mt_file_t* file, mt_settings_t* settings) {
  return NULL;
}

static void skip_line_comment(mt_file_t* file);
static void skip_block_comment(mt_file_t* file);
static mt_token_t* read_identifier(mt_file_t* file, mt_settings_t* settings);
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
      next_n(f, 2);
      skip_line_comment(f);
      continue;
    }
    else if (START(f->ptr, "/*")) {
      next_n(f, 2);
      skip_block_comment(f);
      continue;
    }
    else if (isalpha(c) || c == '_') {
      t = read_identifier(f, s);
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
  
  return t;
}

static void skip_line_comment(mt_file_t* f) {

}

static void skip_block_comment(mt_file_t* f) {

}

static mt_token_t* read_identifier(mt_file_t* f, mt_settings_t* s) {

}

static mt_token_t* read_numeric_literal(mt_file_t* f, mt_settings_t* s) {

}

static mt_token_t* read_string_literal(mt_file_t* f, mt_settings_t* s) {

}

static mt_token_t* read_char_literal(mt_file_t* f, mt_settings_t* s) {

}

static bool is_punctuator(const mt_file_t* f) {

}

static mt_token_t* read_punctuator(mt_file_t* f, mt_settings_t* s) {

}

static mt_token_t* read_eof(mt_file_t* f) {

}

static void read_keyword(mt_token_t* t) {

}

static char next(mt_file_t* f) {
  if (is_newline(f)) {
    ++f->cur_line;
    f->cur_col = 1;
  }
  else {
    ++f->cur_col;
  }
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
  if (c == '\r' && *(f->ptr + 1) == '\n') {
    ++f->cur_col;
    ++f->cur_pos;
    ++f->ptr;
  }
  return true;
}
