#ifndef __MT_LEXER_PUNCTUATOR_H__
#define __MT_LEXER_PUNCTUATOR_H__

#include <mutiny/mutiny.h>

struct _mt_file;
struct _mt_token;

/**
 * @brief Checks if the next token is a punctuator.
 * @param file The file to read from.
 * @return True if the next token is a punctuator.
 */
bool mt_is_punctuator(const struct _mt_file* file);

/**
 * @brief Tokenizes a punctuator.
 * @param file The file to read from.
 * @return The punctuator token.
 */
struct _mt_token* mt_tokenize_punctuator(struct _mt_file* file);

#endif // __MT_LEXER_PUNCTUATOR_H__