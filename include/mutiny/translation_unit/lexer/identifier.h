#ifndef __MT_LEXER_IDENTIFIER_H__
#define __MT_LEXER_IDENTIFIER_H__

struct _mt_file;
struct _mt_token;

/**
 * @brief Tokenizes an identifier.
 * @param file The file to read from.
 * @return The identifier token.
 */
struct _mt_token* mt_tokenize_identifier(struct _mt_file* file);

#endif // __MT_LEXER_IDENTIFIER_H__