#ifndef __MT_SETTINGS_H__
#define __MT_SETTINGS_H__

#include <mutiny/util/list.h>
#include <mutiny/exit_code.h>
#include <stdbool.h>

/**
 * @brief Target operating system.
 */
typedef enum _mt_os {
  OS_UNKNOWN = 0,
  OS_WINDOWS,
  OS_DARWIN,
  OS_LINUX,
  OS_NONE,
} mt_os_t;

/**
 * @brief Target architecture.
 */
typedef enum _mt_arch {
  ARCH_UNKNOWN = 0,
  ARCH_X86_64,
  ARCH_I686,
  ARCH_AARCH64,
  ARCH_ARMV6,
  ARCH_ARMV7,
  ARCH_WASM32,
  ARCH_WASM64,
} mt_arch_t;

/**
 * @brief Stage the compiler is currently executing.
 */
typedef enum _mt_stage {
  STAGE_UNKNOWN = 0,
  STAGE_ARGS,
  STAGE_PARSE,
  STAGE_SEM,
  STAGE_GEN_IR,
  STAGE_GEN_BC,
  STAGE_GEN_ASM,
  STAGE_ASM,
  STAGE_RUN_LINK,
} mt_stage_t;

/**
 * @brief The type of the compiler (jit or non-jit).
 */
typedef enum _mt_compiler_type {
  JIT = 0,
  COMPILER,
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
