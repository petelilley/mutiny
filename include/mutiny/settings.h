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
  STAGE_SCAN_DEPENDICIES,
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
  ANY      = -1,
  JIT      = 0,
  COMPILER = 1,
  PARSER   = 2,
} mt_compiler_type_t;

/**
 * @brief Represents the compiler's runtime settings.
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

#endif // __MT_SETTINGS_H__
