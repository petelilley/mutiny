#ifndef __MT_TRANSLATION_UNIT_H__
#define __MT_TRANSLATION_UNIT_H__

#include <mutiny/mutiny.h>
#include <mutiny/util/list.h>
#include <mutiny/error/error_reporter.h>

struct _mt_settings;
struct _mt_file;
struct _mt_token;
struct _mt_ast_node;

/**
 * @brief A translation unit.
 */
typedef struct _mt_translation_unit {
  struct _mt_settings* settings;
  struct _mt_file* file;
  struct _mt_token* tokens;
  struct _mt_ast_node* ast;
  mt_error_reporter_t error_reporter;
} mt_translation_unit_t;

/**
  * @brief Creates a new translation unit.
  * @param file The file to parse.
  * @param settings The compiler settings to use.
  * @return A new translation unit.
  */
mt_translation_unit_t* mt_translation_unit_init(struct _mt_file* file, struct _mt_settings* settings);

/**
  * @brief Destroys a translation unit.
  * @param unit The translation unit to destroy.
  */
void mt_translation_unit_deinit(mt_translation_unit_t* translation_unit);

/**
  * @brief Parses a translation unit.
  * @param translation_unit The translation unit to parse.
  * @return Whether the translation unit was parsed successfully.
  */
bool mt_translation_unit_parse_exec(mt_translation_unit_t* translation_unit);

#endif  // __MT_TRANSLATION_UNIT_H__
