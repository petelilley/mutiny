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

/**
  * @brief Compares a token's kind.
  */
static inline bool mt_tok_comp(mt_token_t* token, mt_token_kind_t kind) {
  return token->kind == kind;
}

/**
  * @brief Checks if the token is a keyword and compares its keyword value.
  */
static inline bool mt_tok_kw_comp(mt_token_t* token, mt_keyword_t keyword) {
  return mt_tok_comp(token, TK_KEYWORD) && token->ival == keyword;
}

/**
  * @brief Checks if the token is a punctuator and compares its punctuator value.
  */
static inline bool mt_tok_punct_comp(mt_token_t* token, const char* punct) {
  return mt_tok_comp(token, TK_PUNCTUATOR) && strncmp(token->strval, punct, token->len) == 0;
}

/**
  * @brief Compares a token's kind. Logs a syntax error upon failure.
  */
bool mt_tok_match(mt_token_t* token, mt_token_kind_t kind);

/**
  * @brief Checks if the token is a keyword and compares its keyword value. Logs a syntax error upon failure.
  */
bool mt_tok_kw_match(mt_token_t* token, mt_keyword_t keyword);

/**
  * @brief Checks if the token is a punctuator and compares its punctuator value. Logs a syntax error upon failure.
  */
bool mt_tok_punct_match(mt_token_t* token, const char* punct);

#endif // __MT_PARSER_UTIL_H__
