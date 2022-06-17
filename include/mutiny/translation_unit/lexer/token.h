#ifndef __MT_TOKEN_H__
#define __MT_TOKEN_H__

#include <mutiny/mutiny.h>

struct _mt_file;
struct _mt_settings;

/**
 * @brief The kind of a token.
 */
typedef enum _mt_token_kind {
  TK_UNKNOWN    = 0,
  TK_IDENTIFIER = 1 << 0,
  TK_KEYWORD    = 1 << 1,
  TK_PUNCTUATOR = 1 << 2,
  TK_INTEGER    = 1 << 3,
  TK_FLOAT      = 1 << 4,
  TK_STRING     = 1 << 5,
  TK_CHAR       = 1 << 6,
  TK_EOF        = 1 << 7,
} mt_token_kind_t;

/**
 * @brief A keyword in the language.
 */
typedef enum _mt_keyword {
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
 * @brief Converts a keyword to a string.
 * @param keyword The keyword to convert.
 * @return The string representation of the keyword.
 */
const char* mt_keyword_to_str(mt_keyword_t keyword);

/**
 * @brief Converts a string to a keyword.
 * @param str The string to convert.
 * @return The keyword representation of the string.
 */
int mt_str_to_keyword(const char* str, size_t len);

/**
 * @brief Converts a token kind to a string.
 * @param kind The token kind to convert.
 * @return The string representation of the token kind.
 */
const char* mt_token_kind_to_str(mt_token_kind_t kind);

/**
 * @brief A token in the language.
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
  
  // The length of the token.
  size_t len;
  
  struct _mt_file* file;
  
  // The position of the token.
  long int line, col, fpos;
  
  // The beginning of the linked list.
  struct _mt_token* first;
} mt_token_t;

/**
 * @brief Creates a new token.
 * @param file The file where the token is located.
 * @return A new token.
 */
mt_token_t* mt_token_init(struct _mt_file* file);

/**
 * @brief Destroys a token.
 * @param token The token to destroy.
 */
void mt_token_deinit(mt_token_t* token);

#endif // __MT_TOKEN_H__
