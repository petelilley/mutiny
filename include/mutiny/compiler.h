#ifndef __MT_COMPILER_H__
#define __MT_COMPILER_H__

#include <mutiny/mutiny.h>
#include <mutiny/util/list.h>

struct _mt_settings;
struct _mt_file;
struct _mt_translation_unit;
struct _mt_ast_node;

/**
 * @brief Data structure representing a compiler.
 */
typedef struct _mt_compiler {
  struct _mt_settings* settings;
  list_t(struct _mt_file*) files;
  list_t(struct _mt_translation_unit*) t_unit;
  struct _mt_ast_node* ast;
} mt_compiler_t;

/**
 * @brief Creates a new compiler.
 * @param settings The compiler settings to use.
 * @return A new compiler.
 */
mt_compiler_t* mt_compiler_init(struct _mt_settings* settings);

/**
 * @brief Destroys a compiler.
 * @param compiler The compiler to destroy.
 */
void mt_compiler_deinit(mt_compiler_t* compiler);

/**
 * @brief Executes the parsing stage of the compiler.
 * @param compiler The compiler to run.
 */
bool mt_compiler_parse_exec(mt_compiler_t* compiler);

/**
 * @brief Executes the semantic analysis stage of the compiler.
 * @param compiler The compiler to run.
 */
bool mt_compiler_semantic_analysis_exec(mt_compiler_t* compiler);

/**
  * @brief Executes the LLVM IR code generation stage of the compiler.
  * @param compiler The compiler to run.
  */
bool mt_compiler_irgen_exec(mt_compiler_t* compiler);

#endif  // __MT_COMPILER_H__
