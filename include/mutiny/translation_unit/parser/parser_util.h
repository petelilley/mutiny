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
  return mt_tok_comp(token, TK_KEYWORD) && token->kw_val == keyword;
}

/**
  * @brief Checks if the token is a punctuator and compares its punctuator value.
  * @param token The token to compare.
  * @param punct The punctuator to compare.
  * @return True if the token is a punctuator and its punctuator value matches the given punctuator, false otherwise.
  */
static inline bool mt_tok_punct_comp(mt_token_t* token, mt_punctuator_t punct) {
  return mt_tok_comp(token, TK_PUNCTUATOR) && token->punct_val == punct;
}

/**
 * @brief Compares a token's kind. Logs a syntax error upon failure.
 * @param error_reporter The translation unit's error reporter.
 * @param token The token to compare.
 * @param kind_num The number of kinds to compare.
 * @param ... The kinds to compare.
 * @return The matching kind, or TK_UNKNOWN if no match was found.
 */
mt_token_kind_t mt_tok_match(struct _mt_error_reporter* error_reporter, mt_token_t* token, size_t kind_num, ...);

/**
  * @brief Checks if the token is a keyword and compares its keyword value. Logs a syntax error upon failure.
  * @param error_reporter The translator's error reporter.
  * @param token The token to check.
  * @param kw_num The number of keywords to compare.
  * @param ... The keywords to compare.
  * @return The matching keyword, or KW_UNKNOWN if no match was found.
  */
mt_keyword_t mt_tok_kw_match(struct _mt_error_reporter* error_reporter, mt_token_t* token, size_t kw_num, ...);

/**
  * @brief Checks if the token is a punctuator and compares its punctuator value. Logs a syntax error upon failure.
  * @param error_reporter The translator's error reporter.
  * @param token The token to check.
  * @param punct_num The number of punctuators to compare.
  * @return The matching punctuator, or PCT_UNKNOWN if no match was found.
  */
mt_punctuator_t mt_tok_punct_match(struct _mt_error_reporter* error_reporter, mt_token_t* token, size_t punct_num, ...);

#endif // __MT_PARSER_UTIL_H__
