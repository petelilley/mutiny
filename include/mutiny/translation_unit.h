#ifndef __MT_TRANSLATION_UNIT_H__
#define __MT_TRANSLATION_UNIT_H__

#include <mutiny/mutiny.h>
#include <mutiny/util/list.h>

struct _mt_settings;
struct _mt_file;
struct _mt_token;
struct _mt_ast;

/**
 * @brief Data regarding the status of a translation unit.
 *
 * @see mt_settings_t
 * @see mt_file_t
 * @see mt_token_t
 * @see mt_ast_t
 */
typedef struct _mt_translation_unit {
  struct _mt_settings* settings;
  list_t(struct _mt_file*) files;
  struct _mt_token* tokens;
  struct _mt_ast* ast;
} mt_translation_unit_t;

/**
 * @brief Initializes the mutiny translation unit.
 *
 * @param settings The compiler's runtime settings.
 *
 * @return A struct containing data regarding the status of a translation unit. NULL if error.
 *
 * @see mt_settings_t
 * @see mt_translation_unit_t
 */
mt_translation_unit_t* translation_unit_init(struct _mt_settings* settings);

/**
 * @brief De-initializes the translation unit and frees all allocated memory contained by it.
 *
 * @param translation_unit The translation unit to be de-initialized.
 *
 * @see mt_translation_unit_t
 */
void translation_unit_deinit(mt_translation_unit_t* translation_unit);

/**
 * @brief Executes the lexical analysis and parsing stages of the translation unit.
 *
 * @param translation_unit The translation unit to be executed.
 *
 * @return Whether the translation executed successfully.
 *
 * @see mt_translation_unit_t
 */
bool translation_unit_parse_exec(mt_translation_unit_t* translation_unit);

/**
 * @brief Executes the semantic analysis stage of the translation unit.
 *
 * @param translation_unit The translation unit to be executed.
 *
 * @return Whether the translation executed successfully.
 *
 * @see mt_translation_unit_t
 */
bool translation_unit_semantic_analysis_exec(mt_translation_unit_t* translation_unit);

/**
 * @brief Executes the LLVM IR generation stage of the translation unit.
 *
 * @param translation_unit The translation unit to be executed.
 *
 * @return Whether the translation executed successfully.
 *
 * @see mt_translation_unit_t
 */
bool translation_unit_irgen_exec(mt_translation_unit_t* translation_unit);

#endif  // __MT_TRANSLATION_UNIT_H__
