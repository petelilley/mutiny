#ifndef __MT_PARSER_UTIL_H__
#define __MT_PARSER_UTIL_H__

#include <mutiny/mutiny.h>

/**
 * @brief Compares the start of a string to another string.
 */
#define MT_STR_START(x, y) (strncmp((x), (y), strlen(y)) == 0)

/**
 * @brief Compares the end of a string to another string.
 */
#define MT_STR_END(x, y)   ((strlen(x) >= strlen(y)) && (strncmp((x) + strlen(x) - strlen(y), (y), strlen(y)) == 0))

#include <mutiny/translation_unit/lexer/token.h>

struct _mt_error_reporter;

/**
  * @brief Compares a token's kind.
  * @param token The token to compare.
  * @param kind The kind to compare.
  * @return True if the token's kind matches the given kind, false otherwise.
  */
static inline bool mt_tok_comp(mt_token_t* token, mt_token_kind_t kind) {
  return token->kind == kind;
}

/**
  * @brief Checks if the token is a keyword and compares its keyword value.
  * @param token The token to compare.
  * @param keyword The keyword to compare.
  * @return True if the token is a keyword and its keyword value matches the given keyword, false otherwise.
  */
static inline bool mt_tok_kw_comp(mt_token_t* token, mt_keyword_t keyword) {
  return mt_tok_comp(token, TK_KEYWORD) && token->ival == keyword;
}

/**
  * @brief Checks if the token is a punctuator and compares its punctuator value.
  * @param token The token to compare.
  * @param punct The punctuator to compare.
  * @return True if the token is a punctuator and its punctuator value matches the given punctuator, false otherwise.
  */
static inline bool mt_tok_punct_comp(mt_token_t* token, const char* punct) {
  return mt_tok_comp(token, TK_PUNCTUATOR) && strncmp(token->strval, punct, token->len) == 0;
}

/**
 * @brief Compares a token's kind. Logs a syntax error upon failure.
 * @param token The token to compare.
 * @param kind The kind to compare.
 * @param error_reporter The translation unit's error reporter.
 * @return True if the token's kind matches the given kind, false otherwise.
 */
bool mt_tok_match(mt_token_t* token, mt_token_kind_t kind, struct _mt_error_reporter* error_reporter);

/**
  * @brief Checks if the token is a keyword and compares its keyword value. Logs a syntax error upon failure.
  * @param token The token to check.
  * @param keyword The keyword to compare to.
  * @param error_reporter The translator's error reporter.
  * @return True if the token's kind matches keyword and its keyword matches the given keyword, false otherwise.
  */
bool mt_tok_kw_match(mt_token_t* token, mt_keyword_t keyword, struct _mt_error_reporter* error_reporter);

/**
  * @brief Checks if the token is a punctuator and compares its punctuator value. Logs a syntax error upon failure.
  * @param token The token to check.
  * @param punct The punctuator to compare to.
  * @param error_reporter The translator's error reporter.
  * @return True if the token's kind matches punctuator and its punctuator matches the given punctuator, false otherwise.
  */
bool mt_tok_punct_match(mt_token_t* token, const char* punct, struct _mt_error_reporter* error_reporter);

#endif // __MT_PARSER_UTIL_H__
