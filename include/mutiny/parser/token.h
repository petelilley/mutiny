#ifndef __MT_TOKEN_H__
#define __MT_TOKEN_H__

#include <mutiny/mutiny.h>

struct _mt_file;
struct _mt_settings;

/**
 * @brief The kind of token in the mutiny source code.
 */
typedef enum _mt_token_kind {
  TK_IDENTIFIER = 0,
  TK_KEYWORD,
  TK_PUNCTUATOR,
  TK_INTEGER,
  TK_FLOAT,
  TK_STRING,
  TK_CHAR,
  TK_EOF,
} mt_token_kind_t;

/**
 * @brief A keyword in the mutiny source code.
 */
typedef enum {
  KW_IMPORT = 0,
  KW_NOT, KW_AND, KW_OR,
  KW_ENUM, KW_STRUCT, KW_FUNC, KW_VAR,
  KW_IF, KW_ELIF, KW_ELSE, KW_WHILE, KW_FOR, KW_SWITCH, KW_GOTO, // Statements.
  KW_CASE, KW_DEFAULT,
  KW_RETURN, KW_BREAK, KW_CONTINUE,
  /*
  KW_STATIC, // Storage class specifiers.
  KW_CONST, KW_VOLATILE, // Type qualifiers.
  KW_INLINE,
  */
  KW_SIGNED, KW_UNSIGNED,
} mt_keyword_t;

/**
 * @brief Converts a keyword into a string.
 *
 * @param keyword The keyword to be converted.
 *
 * @return The string representation of the keyword.
 *
 * @see mt_keyword_t
 */
const char* mt_keyword_to_str(mt_keyword_t keyword);

/**
 * @brief Attempts to convert a string into a keyword.
 *
 * @param str The string to convert.
 * @param len The length of the string to convert.
 *
 * @return The keyword if the string matches, -1 if not.
 *
 * @see mt_keyword_t
 */
int mt_str_to_keyword(const char* str, size_t len);

/**
 * @brief Represents a token.
 */
typedef struct _mt_token {
  // The kind of token.
  mt_token_kind_t kind;
  
  // The next token in the linked list.
  struct _mt_token* next;
  
  // The value of the token.
  char cval;
  long long int ival;
  long double fval;
  char* strval;
  
  // The length of the token in the source code.
  size_t len;
  
  // The position of the token in the source file.
  long int line, col, fpos;
  
  // The beginning of the linked list.
  struct _mt_token* first;
} mt_token_t;

mt_token_t* mt_token_init(struct _mt_file* file);

void mt_token_deinit(mt_token_t* token);

#endif // __MT_TOKEN_H__
