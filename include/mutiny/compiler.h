#ifndef __MT_COMPILER_H__
#define __MT_COMPILER_H__

#include <mutiny/mutiny.h>
#include <mutiny/util/list.h>

struct _mt_settings;
struct _mt_file;
struct _mt_translation_unit;
struct _mt_ast;

/**
 * @brief Data regarding the status of a compiler.
 *
 * @see mt_settings_t
 * @see mt_file_t
 * @see mt_translation_unit
 * @see mt_ast_t
 */
typedef struct _mt_compiler {
  struct _mt_settings* settings;
  list_t(struct _mt_file*) files;
  list_t(struct _mt_translation_unit*) t_unit;
  struct _mt_ast* ast;
} mt_compiler_t;

/**
 * @brief Initializes the mutiny compiler.
 *
 * @param settings The compiler's runtime settings.
 *
 * @return A new compiler. NULL if error.
 *
 * @see mt_settings_t
 * @see mt_compiler_t
 */
mt_compiler_t* mt_compiler_init(struct _mt_settings* settings);

/**
 * @brief De-initializes the compiler and frees all allocated memory contained by it.
 *
 * @param compiler The compiler to be de-initialized.
 *
 * @see mt_compiler_t
 */
void mt_compiler_deinit(mt_compiler_t* compiler);

/**
 * @brief Executes the lexical analysis and parsing stages of the compiler.
 *
 * @param compiler The compiler to be executed.
 *
 * @return Whether the translation executed successfully.
 *
 * @see mt_compiler_t
 */
bool mt_compiler_parse_exec(mt_compiler_t* compiler);

/**
 * @brief Executes the semantic analysis stage of the compiler.
 *
 * @param compiler The compiler to be executed.
 *
 * @return Whether the translation executed successfully.
 *
 * @see mt_compiler_t
 */
bool mt_compiler_semantic_analysis_exec(mt_compiler_t* compiler);

/**
 * @brief Executes the LLVM IR generation stage of the compiler.
 *
 * @param compiler The compiler to be executed.
 *
 * @return Whether the translation executed successfully.
 *
 * @see mt_compiler_t
 */
bool mt_compiler_irgen_exec(mt_compiler_t* compiler);

#endif  // __MT_COMPILER_H__
