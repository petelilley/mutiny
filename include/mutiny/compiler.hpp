#pragma once

#include <mutiny/mutiny.hpp>
#include <mutiny/util/logger.hpp>
#include <mutiny/code_gen/code_gen.hpp>

namespace mt {

class Compiler {
public:
  static inline Compiler& get() {
    static Compiler instance;
    return instance;
  }

  /**
   * @brief Decodes the command line arguments and sets up the compiler.
   * 
   * @param argc The number of command line arguments.
   * @param argv The command line arguments.
   */
  void setup(u32 argc, const char** argv);

  /**
   * @brief Executes the compiler.
   * 
   * @return The program's exit code.
   */
  s32 exec();
  
private:
  Compiler();
  ~Compiler();

  /**
   * @brief Decodes the command line arguments.
   * 
   * @param args The command line arguments.
   */
  void decode_args(std::vector<const char*>& args);

private:
  enum class ExitCode {
    SUCCESS           = 0,
    INTERNAL_ERR      = 1,
    INVALID_ARGS      = 2,
    INVALID_FILE      = 3,
    INVALID_TOKENS    = 4,
    INVALID_SYNTAX    = 5,
    INVALID_SEMANTICS = 6,
    INVALID_OS        = 7,
    INVALID_ARCH      = 8,
  };
  
  // The exit code of the program.
  ExitCode exit_code = ExitCode::SUCCESS;

  b8 should_exit = false;

  Logger log_out;
  Logger log_err;
  Logger log_warn;

  enum class Mode {
    UNKNOWN = 0,
    JIT,
    COMPILE,
    PARSE,
    USAGE,
    VERSION,
  };

  // The mode of the compiler.
  Mode mode = Mode::UNKNOWN;
  
  enum class OS {
    UNKNOWN = 0,
    NONE,
    WINDOWS,
    LINUX,
    MACOS,
  };
  
  // The operating system.
  OS os = OS::UNKNOWN;

  enum class Arch {
    UNKNOWN = 0,
    X86_64,
    I686,
    AARCH64,
    ARMV6,
    ARMV7,
    WASM32,
    WASM64,
  };
  
  // The architecture.
  Arch arch = Arch::UNKNOWN;

  enum class Stage {
    UNKNOWN = 0,
    ARGS,
    SCAN_DEPENDENCIES,
    PARSE,
    SEMANTIC_ANALYSIS,
    IR_GEN,
    BC_GEN,
    ASM_GEN,
    ASSEMBLE,
    LINK,
    RUN,
  };
  
  // The stage of the compiler.
  Stage stage = Stage::UNKNOWN;

  b8 emit_asm   = false;
  b8 emit_irgen = false;
  b8 emit_ir    = false;
  b8 emit_bc    = false;
  b8 emit_obj   = false;
  b8 emit_exe   = false;
  b8 emit_lib   = false;

  b8 print_ast      = false;
  b8 verbose_output = false;
  b8 debug_symbols  = false;
  b8 optimize       = false;
  b8 no_warnings    = false;
  b8 error_warnings = false;
  u32 job_num       = 0;

  // The path to the output file.
  std::filesystem::path output_path = std::filesystem::current_path() /
#ifdef MT_OS_WINDOWS
  "a.exe";
#else
  "a.out";
#endif

  // Search paths for libraries.
  std::vector<std::filesystem::path> lib_paths;
  // Libraries to link.
  std::vector<std::filesystem::path> link_libs;
  // The source files.
  std::vector<std::filesystem::path> src_paths;

  // SemanticAnalyzer sema_analyzer;
  CodeGen code_gen;
  // CodeRunner code_runner;
  // Linker linker;
};

} // namespace mt