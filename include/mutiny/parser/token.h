#ifndef __MT_TOKEN_H__
#define __MT_TOKEN_H__

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
  KW_ENUM, KW_STRUCT, KW_UNION, KW_FUNC, KW_VAR,
  KW_IF, KW_ELIF, KW_ELSE, KW_WHILE, KW_FOR, KW_SWITCH, KW_GOTO, // Statements.
  KW_CASE, KW_DEFAULT,
  KW_RETURN, KW_BREAK, KW_CONTINUE,
  KW_AUTO, KW_REGISTER, KW_STATIC,
  KW_CONST, KW_VOLATILE, // Type qualifiers.
  KW_INLINE,
  KW_SIGNED, KW_UNSIGNED,
} mt_keyword_t;

#include <stddef.h>

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

#endif // __MT_TOKEN_H__
