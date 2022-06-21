#ifndef __MT_PARSER_UTIL_H__
#define __MT_PARSER_UTIL_H__

#include <mutiny/mutiny.h>

/**
 * @brief Checks if a string begins with another string.
 * @param s1 The string to compare.
 * @param s2 The prefix to check for.
 */
#define MT_STR_START(s1, s2) \
  (strncmp((s1), (s2), strlen(s2)) == 0)

/**
 * @brief Checks if a string ends with another string.
 * @param s1 The string to compare.
 * @param s2 The suffix to check for.
 */
#define MT_STR_END(s1, s2) \
  ((strlen(s1) >= strlen(s2)) && (strncmp((s1) + strlen(s1) - strlen(s2), (s2), strlen(s2)) == 0))

#include <mutiny/translation_unit/lexer/token.h>

/**
 * @brief Compares a token's kind.
 * @param t The token to compare.
 * @param k The kind to compare.
 */
#define MT_TOK_COMP(t, k) \
  (t->kind == k)

/**
 * @brief Checks is the token is a keyword and compares its keyword value.
 * @param t The token to compare.
 * @param k The keyword to compare.
 * @return True if the token is a keyword and its keyword value matches the given keyword, false otherwise.
 */
#define MT_TOK_COMP_KW(t, k) \
  (MT_TOK_COMP(t, TK_KEYWORD) && t->kw_val == k)

/**
 * @brief Checks if the token is a punctuator and compares its punctuator value.
 * @param t The token to compare.
 * @param p The punctuator to compare.
 * @return True if the token is a punctuator and its punctuator value matches the given punctuator, false otherwise.
 */
#define MT_TOK_COMP_PUNCT(t, p) \
  (MT_TOK_COMP(t, TK_PUNCTUATOR) && t->punct_val == p)

struct _mt_error_reporter;

/**
 * @brief Compares a token's kind. Logs a syntax error upon failure.
 * @param error_reporter The translation unit's error reporter.
 * @param token The token to compare.
 * @param kind_num The number of kinds to compare.
 * @param ... The kinds to compare.
 * @return The matching kind, or TK_UNKNOWN if no match was found.
 */
mt_token_kind_t mt_token_match(struct _mt_error_reporter* error_reporter, mt_token_t* token, size_t kind_num, ...);

/**
  * @brief Checks if the token is a keyword and compares its keyword value. Logs a syntax error upon failure.
  * @param error_reporter The translator's error reporter.
  * @param token The token to check.
  * @param kw_num The number of keywords to compare.
  * @param ... The keywords to compare.
  * @return The matching keyword, or KW_UNKNOWN if no match was found.
  */
mt_keyword_t mt_token_match_kw(struct _mt_error_reporter* error_reporter, mt_token_t* token, size_t kw_num, ...);

/**
  * @brief Checks if the token is a punctuator and compares its punctuator value. Logs a syntax error upon failure.
  * @param error_reporter The translator's error reporter.
  * @param token The token to check.
  * @param punct_num The number of punctuators to compare.
  * @param ... The punctuators to compare.
  * @return The matching punctuator, or PCT_UNKNOWN if no match was found.
  */
mt_punctuator_t mt_token_match_punct(struct _mt_error_reporter* error_reporter, mt_token_t* token, size_t punct_num, ...);

#endif // __MT_PARSER_UTIL_H__
