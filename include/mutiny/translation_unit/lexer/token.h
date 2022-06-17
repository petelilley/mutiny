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
  TK_IDENTIFIER,
  TK_KEYWORD,
  TK_PUNCTUATOR,
  TK_INTEGER,
  TK_FLOAT,
  TK_STRING,
  TK_CHAR,
  TK_EOF,
} mt_token_kind_t;

/**
 * @brief A keyword in the language.
 */
typedef enum _mt_keyword {
  KW_UNKNOWN = 0,
  KW_IMPORT,
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
 * @brief A punctuator in the language.
 */
typedef enum _mt_punctuator {
  PCT_UNKNOWN = 0,
  PCT_LPAREN,     // (
  PCT_RPAREN,     // )
  PCT_LBRACE,     // {
  PCT_RBRACE,     // }
  PCT_LBRACKET,   // [
  PCT_RBRACKET,   // ]
  PCT_COMMA,      //,
  PCT_SEMICOLON,  // ;
  PCT_COLON,      // :
  PCT_DOT,        // .
  PCT_EQ,         // =
  PCT_PLUS,       // +
  PCT_MINUS,      // -
  PCT_MUL,        // *
  PCT_DIV,        // /
  PCT_MOD,        // %
  PCT_BIT_AND,    // &
  PCT_BIT_OR,     // |
  PCT_BIT_XOR,    // ^
  PCT_BIT_NOT,    // ~
  PCT_BIT_LSH,    // <<
  PCT_BIT_RSH,    // >>
  PCT_PLUS_EQ,    // +=
  PCT_MINUS_EQ,   // -=
  PCT_MUL_EQ,     // *=
  PCT_DIV_EQ,     // /=
  PCT_MOD_EQ,     // %=
  PCT_BIT_AND_EQ, // &=
  PCT_BIT_OR_EQ,  // |=
  PCT_BIT_XOR_EQ, // ^=
  PCT_LSH_EQ,     // <<=
  PCT_RSH_EQ,     // >>=
  PCT_EQ_EQ,      // ==
  PCT_NOT_EQ,     // !=
  PCT_LESS_EQ,    // <=
  PCT_GREATER_EQ, // >=
  PCT_LOG_OR,     // ||
  PCT_LOG_AND,    // &&
  PCT_INC,        // ++
  PCT_DEC,        // --
  PCT_ARROW,      // ->
  PCT_GREATER,    // >
  PCT_LESS,       // <
  PCT_NOT,        // !
  PCT_REF,        // @
  PCT_COL_COL,    // ::
  PCT_QUESTION,   // ?
  PCT_COL_EQ,     // :=
} mt_punctuator_t;

/**
 * @brief Converts a keyword to a string.
 * @param keyword The keyword to convert.
 * @return The string representation of the keyword.
 */
const char* mt_keyword_to_str(mt_keyword_t keyword);

/**
 * @brief Converts a string to a keyword.
 * @param str The string to convert.
 * @param len The length of the string.
 * @return The keyword representation of the string.
 */
ssize_t mt_str_to_keyword(const char* str, size_t len);

/**
 * @brief Converts a token kind to a string.
 * @param kind The token kind to convert.
 * @return The string representation of the token kind.
 */
const char* mt_token_kind_to_str(mt_token_kind_t kind);

/**
 * @brief Converts a punctuator to a string.
 * @param punctuator The punctuator to convert.
 * @return The string representation of the punctuator.
 */
const char* mt_punct_to_str(mt_punctuator_t punct);

/**
 * @brief Converts a string to a punctuator.
 * @param str The string to convert.
 * @param len The length of the string.
 * @return The punctuator representation of the string.
 */
ssize_t mt_str_to_punct(const char* str, size_t len);

/**
 * @brief A token in the language.
 */
typedef struct _mt_token {
  // The kind of token.
  mt_token_kind_t kind;
  
  // The next token in the linked list.
  struct _mt_token* next;
  
  // The value of the token.
  char c_val;
  long long int i_val;
  long double f_val;
  char* str_val;

  mt_keyword_t kw_val;
  mt_punctuator_t punct_val;
  
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
