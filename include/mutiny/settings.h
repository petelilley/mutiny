#ifndef __MT_SETTINGS_H__
#define __MT_SETTINGS_H__

#include <mutiny/util/list.h>
#include <mutiny/exit_code.h>
#include <stdbool.h>

/**
 * @brief Target operating system.
 */
typedef enum _mt_os {
  os_unknown = 0,
  os_windows,
  os_darwin,
  os_linux,
  os_none,
} mt_os_t;

/**
 * @brief Target architecture.
 */
typedef enum _mt_arch {
  arch_unknown = 0,
  arch_x86_64,
  arch_i686,
  arch_aarch64,
  arch_armv6,
  arch_armv7,
  arch_wasm32,
  arch_wasm64,
} mt_arch_t;

/**
 * @brief Stage the compiler is currently executing.
 */
typedef enum _mt_stage {
  stage_unknown = 0,
  stage_args,
  stage_parse,
  stage_sem,
  stage_gen_ir,
  stage_gen_bc,
  stage_gen_asm,
  stage_asm,
  stage_run_link,
} mt_stage_t;

/**
 * @brief The type of the compiler (jit or non-jit).
 */
typedef enum _mt_compiler_type {
  mt_jit = 0,
  mt_comp,
} mt_compiler_type_t;

/**
 * @brief Represents the compiler's runtime settings.
 */
typedef struct _mt_settings {
  mt_exit_code_t exit_code;
  bool end;
  
  mt_compiler_type_t type;
  
  mt_os_t os;
  mt_arch_t arch;
  
  char* output;
  
  list_t(char*) src_dirs;
  list_t(char*) lib_dirs;
  
  bool optimize;
  bool hide_warnings;
  bool print_ast;
  bool verbose;
  
  mt_stage_t stage;
} mt_settings_t;

#endif // __MT_SETTINGS_H__
