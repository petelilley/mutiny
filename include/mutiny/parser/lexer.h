#ifndef __MT_LEXER_H__
#define __MT_LEXER_H__

struct _mt_file;
struct _mt_settings;
struct _mt_token;

/**
 * @brief Tokenizes a singular mutiny source file.
 *
 * @param file The file to tokenize.
 * @param settings The compiler's runtime settings.
 *
 * @return A linked list of the file's sources. NULL if error.
 *
 * @see mt_file_t
 * @see mt_settings_t
 * @see mt_token_t
 */
struct _mt_token* tokenize(struct _mt_file* file, struct _mt_settings* settings);

#endif // __MT_LEXER_H__
