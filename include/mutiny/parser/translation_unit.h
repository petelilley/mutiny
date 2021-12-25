#ifndef __MT_TRANSLATION_UNIT_H__
#define __MT_TRANSLATION_UNIT_H__

struct _mt_settings;
struct _mt_ast;

/**
 * @brief Parses mutiny source code.
 *
 * @param settings The compiler's runtime settings.
 *
 * @return An abstract sytnax tree representing the mutiny source code. NULL if error.
 *
 * @see mt_settings_t
 * @see mt_ast_t
 */
struct _mt_ast* parse(struct _mt_settings* settings);

#endif // __MT_TRANSLATION_UNIT_H__
