#ifndef __MT_SETTINGS_H__
#define __MT_SETTINGS_H__

#include <mutiny/util/list.h>
#include <mutiny/exit_code.h>
#include <stdbool.h>

/**
 * @brief Target operating system.
 */
typedef enum _mt_os {
  MT_OS_UNKNOWN = 0,
  MT_OS_WINDOWS,
  MT_OS_DARWIN,
  MT_OS_LINUX,
  MT_OS_NONE,
} mt_os_t;

/**
 * @brief Target architecture.
 */
typedef enum _mt_arch {
  MT_ARCH_UNKNOWN = 0,
  MT_ARCH_X86_64,
  MT_ARCH_I686,
  MT_ARCH_AARCH64,
  MT_ARCH_ARMV6,
  MT_ARCH_ARMV7,
  MT_ARCH_WASM32,
  MT_ARCH_WASM64,
} mt_arch_t;

/**
 * @brief Compiler stage.
 */
typedef enum _mt_stage {
  MT_STAGE_UNKNOWN = 0,
  MT_STAGE_ARGS,
  MT_STAGE_SCAN_DEPENDICIES,
  MT_STAGE_PARSE,
  MT_STAGE_SEM,
  MT_STAGE_GEN_IR,
  MT_STAGE_GEN_BC,
  MT_STAGE_GEN_ASM,
  MT_STAGE_ASM,
  MT_STAGE_RUN_LINK,
} mt_stage_t;

/**
 * @brief The type of the compiler.
 */
typedef enum _mt_compiler_type {
  MT_COMPILER_ANY      = -1,
  MT_COMPILER_JIT      = 0,
  MT_COMPILER_COMPILER = 1,
  MT_COMPILER_PARSER   = 2,
} mt_compiler_type_t;

/**
 * @brief The compiler's runtime settings.
 */
typedef struct _mt_settings {
  mt_exit_code_t exit_code;
  bool end;
  
  mt_compiler_type_t type;
  
  bool emit_asm;
  bool emit_irgen;
  bool emit_ir;
  bool emit_bc;
  bool emit_obj;
  bool emit_exe;
  bool emit_lib;
  
  mt_os_t os;
  mt_arch_t arch;
  
  bool print_ast;
  bool verbose;
  bool debug;
  unsigned job_num;
  bool optimize;
  bool no_warnings;
  
  list_t(char*) import_paths;
  list_t(char*) lib_paths;
  list_t(char*) link_libs;
  
  char* output;
  
  list_t(char*) src_dirs;
  
  mt_stage_t stage;
} mt_settings_t;

/**
 * @brief Creates a new compiler settings.
 * @return A new compiler settings.
 */
mt_settings_t* mt_settings_init();

/**
 * @brief Destroys a compiler settings.
 * @param settings The compiler settings to destroy.
 */
void mt_settings_deinit(mt_settings_t* settings);

#endif // __MT_SETTINGS_H__
