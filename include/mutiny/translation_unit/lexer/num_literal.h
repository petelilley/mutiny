#ifndef __MT_LEXER_NUMERIC_LITERAL_H__
#define __MT_LEXER_NUMERIC_LITERAL_H__

struct _mt_file;
struct _mt_error_reporter;
struct _mt_token;

/**
 * @brief Tokenizes a numeric literal.
 * @param file The file to read from.
 * @param error_reporter The translation unit's error reporter.
 * @return The numeric literal token.
 */
struct _mt_token* mt_tokenize_numeric_literal(struct _mt_file* file, struct _mt_error_reporter* error_reporter);

#endif // __MT_LEXER_NUMERIC_LITERAL_H__