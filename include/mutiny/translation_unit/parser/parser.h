#ifndef __MT_PARSER_H__
#define __MT_PARSER_H__

#include <mutiny/mutiny.h>

struct _mt_translation_unit;

/**
 * @brief Parses a translation unit.
 * @param translation_unit The translation unit to parse.
 * @return Whether the translation unit was parsed successfully.
 */
bool mt_translation_unit_parse_tokens(struct _mt_translation_unit* translation_unit);

#endif // __MT_PARSER_H__
