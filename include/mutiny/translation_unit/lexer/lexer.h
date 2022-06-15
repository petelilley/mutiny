#ifndef __MT_LEXER_H__
#define __MT_LEXER_H__

#include <mutiny/mutiny.h>

struct _mt_translation_unit;

/**
 * @brief Tokenizes a translation unit.
 * @param translation_unit The translation unit to tokenize.
 */
bool mt_translation_unit_tokenize(struct _mt_translation_unit* translation_unit);

#endif // __MT_LEXER_H__
