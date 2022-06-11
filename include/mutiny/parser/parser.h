#ifndef __MT_PARSER_H__
#define __MT_PARSER_H__

struct _mt_token;
struct _mt_settings;
struct _mt_ast;

/**
 * @brief Parses tokens representing mutiny source code.
 *
 * @param tokens A linked list of tokens.
 * @param settings The compiler's runtime settings.
 *
 * @return An abstract sytnax tree representing the source code. NULL if error.
 *
 * @see mt_token_t
 * @see mt_settings_t
 * @see mt_ast_t
 */
struct _mt_ast* mt_parse_tokens(struct _mt_token* tokens, struct _mt_settings* settings);

#endif // __MT_PARSER_H__
