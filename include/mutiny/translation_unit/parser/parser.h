#ifndef __MT_PARSER_H__
#define __MT_PARSER_H__

#include <mutiny/mutiny.h>

struct _mt_translation_unit;

/**
 * @brief Parses tokens of a translation unit.
 *
 * @param translation_unit the translation unit to be tokenized.
 *
 * @return Whether the parser parsed the tokens successfully.
 *
 * @see mt_translation_unit_t
 */
bool mt_translation_unit_parse_tokens(struct _mt_translation_unit* translation_unit);

#endif // __MT_PARSER_H__
