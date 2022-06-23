#include <mutiny/mutiny.hpp>
#include <mutiny/compiler.hpp>
#include <mutiny/args.hpp>

#define ARGS_ERR_TAG \
  LogStyle::BOLD << LogStyle::RED << "Arguments: " << LogStyle::CLEAR

using namespace mt;

enum Command : u32 {
  UNKNOWN = 0,
  PARSE   = (1 << 0),
  RUN     = ((1 << 1) | PARSE),
  BUILD   = ((1 << 2) | PARSE),
  COMPILE = (RUN | BUILD),
  VERSION = (1 << 3),
  HELP    = (1 << 4),
  ALL     = (COMPILE | VERSION | HELP),
};

enum {
  OPT_EMIT_ASSEMBLY,
  OPT_EMIT_IRGEN,
  OPT_EMIT_IR,
  OPT_EMIT_BC,
  OPT_EMIT_OBJECT,
  OPT_EMIT_EXECUTABLE,
  OPT_EMIT_LIBRARY,
  OPT_OS,
  OPT_ARCH,
  OPT_PRINT_AST,
  OPT_VERBOSE,
  OPT_DEBUG,
  OPT_JOB_NUM,
  OPT_LIBRARY_PATH,
  OPT_LINK_LIBRARY,
  OPT_OPTIMIZE,
  OPT_OUTPUT,
  OPT_WNO,
  OPT_WERROR,
};

static constexpr std::array<Option, 19> options {{
  { OPT_EMIT_ASSEMBLY,   "emit-assembly",  'S', false, Command::BUILD,   "Emit assembly file(s) (-S)."                         },
  { OPT_EMIT_IRGEN,      "emit-irgen",      0,  false, Command::BUILD,   "Emit LLVM IR file(s) before LLVM optimizations."     },
  { OPT_EMIT_IR,         "emit-ir",         0,  false, Command::BUILD,   "Emit LLVM IR file(s) after LLVM optimizations."      },
  { OPT_EMIT_BC,         "emit-bc",         0,  false, Command::BUILD,   "Emit LLVM BC file(s)."                               },
  { OPT_EMIT_OBJECT,     "emit-object",    'c', false, Command::BUILD,   "Emit object file(s) (-c)."                           },
  { OPT_EMIT_EXECUTABLE, "emit_executable", 0,  false, Command::BUILD,   "Emit a linked executable."                           },
  { OPT_EMIT_LIBRARY,    "emit_library",    0,  false, Command::BUILD,   "Emit a linked library."                              },
  { OPT_OS,              "os",              0,  true,  Command::BUILD,   "Specify the target operating system."                },
  { OPT_ARCH,            "arch",            0,  true,  Command::BUILD,   "Specify the target architecture."                    },
  { OPT_PRINT_AST,       "print-ast",       0,  false, Command::PARSE,   "Print generated AST(s)."                             },
  { OPT_VERBOSE,         "verbose",        'v', false, Command::ALL,     "Use verbose output."                                 },
  { OPT_DEBUG,           "debug",          'g', false, Command::BUILD,   "Emit debug symbols."                                 },
  { OPT_JOB_NUM,         "jobs",           'j', true,  Command::PARSE,   "Max number of jobs to execute in parallel."          },
  { OPT_LIBRARY_PATH,    "library-path",   'L', true,  Command::PARSE,   "Add path to library search path."                    },
  { OPT_LINK_LIBRARY,    "link-library",   'l', true,  Command::PARSE,   "Specifies a library which should be linked against." },
  { OPT_OPTIMIZE,        "optimize",       'O', false, Command::COMPILE, "Compile with optimizations."                         },
  { OPT_OUTPUT,          "output",         'o', true,  Command::BUILD,   "Specify file to write output to."                    },
  { OPT_WNO,             "Wno",            'w', false, Command::PARSE,   "Suppress all warnings (-w)."                         },
  { OPT_WERROR,          "Werror",          0,  false, Command::PARSE,   "Treat all warnings as errors."                       },
}};

void Compiler::decode_args(std::vector<const char*>& args) {
  auto show_usage = [&](Logger& log, Command cmd) {
    if (cmd == Command::VERSION || cmd == Command::HELP) {
      cmd = Command::UNKNOWN;
    }
    if (cmd == Command::UNKNOWN) {
      log << LogStyle::BOLD << "Usage:" << LogStyle::CLEAR << " mutiny [command] [files...] [options]\n";
      log << LogStyle::BOLD << "\nAvailable commands:\n"
        << LogStyle::BOLD << "  run                         " << LogStyle::CLEAR << "Compile and run a Mutiny program.\n"
        << LogStyle::BOLD << "  build                       " << LogStyle::CLEAR << "Compile a Mutiny program.\n"
        << LogStyle::BOLD << "  parse                       " << LogStyle::CLEAR << "Parse a Mutiny program.\n"
        << LogStyle::BOLD << "  version                     " << LogStyle::CLEAR << "Print the compiler version and exit.\n"
        << LogStyle::BOLD << "  help                        " << LogStyle::CLEAR << "Print this usage information and exit.\n"
        "\nRun " << LogStyle::BOLD << "`mutiny help <command>'" << LogStyle::CLEAR << " for information about a specific command.\n";
      return;
    }
    
    const char* cmd_usage;
    
    switch (cmd) {
      case Command::BUILD:
        cmd_usage = "mutiny build [files...] [-o output] [options]\n";
        break;
      default:
        cmd_usage = "mutiny run [files...] [options]\n";
        break;
    }
    log << LogStyle::BOLD << "Usage: " << cmd_usage << LogStyle::CLEAR;

    for (auto& opt : options) {
      if (opt.context & ~cmd) continue;
      
      u32 ws = 30 - std::strlen(opt.name) - 3;
      
      log << "  -" << opt.name;
      if (opt.requires_arg) {
        log << LogStyle::BOLD << " <arg>" << LogStyle::CLEAR;
        ws -= 6;
      }
      for (u32 i = 0; i < ws; i++) {
        log << " ";
      }
      log << opt.help << "\n";
    }
  };

  if (args.size() < 2) {
    log_out << LogStyle::BOLD << "Mutiny is a tool for managing mutiny source code.\n\n" << LogStyle::CLEAR;
    show_usage(log_out, Command::UNKNOWN);
    should_exit = true;
    return;
  }
  
  auto get_cmd = [](std::string_view c) -> Command {
    if      (c == "run"   || c == "jit")     { return Command::RUN;     }
    else if (c == "build" || c == "compile") { return Command::BUILD;   }
    else if (c == "parse")                   { return Command::PARSE;   }
    else if (c == "version")                 { return Command::VERSION; }
    else if (c == "help" || c == "usage")    { return Command::HELP;    }
    else                                     { return Command::UNKNOWN; }
  };

  Command cmd = get_cmd(args.at(1));
  
  switch(cmd) {
    case Command::PARSE:
      mode = Mode::PARSE;
      break;
    case Command::RUN:
      mode = Mode::JIT;
      break;
    case Command::BUILD:
      mode = Mode::COMPILE;
      break;
    case Command::VERSION:
      log_out << "Mutiny version " << MT_VERSION_FULL_STR << ' ' << MT_HOST_OS_STR << '/' << MT_HOST_ARCH_STR << "\n";
      should_exit = true;
      return;
    case Command::HELP:
      should_exit = true;
      if (args.size() > 2) {
        show_usage(log_out, get_cmd(args.at(2)));
      }
      else {
        show_usage(log_out, Command::UNKNOWN);
      }
      return;
    default:
      log_err << ARGS_ERR_TAG << LogStyle::BOLD << "Unknown command `" << args.at(1) << "'\n" << LogStyle::CLEAR;
      show_usage(log_err, Command::UNKNOWN);
      should_exit = true;
      exit_code = ExitCode::INVALID_ARGS;
      return;
  }
  
  std::vector<InputOption> input_opts;
  
  for (u32 arg_index = 1; arg_index < args.size(); arg_index++) {
    std::string arg = args.at(arg_index);
    
    bool valid_opt = false;
    
    if (arg.at(0) != '-') {
      src_dirs.push_back(arg);
      continue;
    }
    
    // Remove the leading '-'.
    arg.erase(0, 1);

    // A second '-'.
    if (!arg.empty() && arg.at(0) == '-') {
      arg.erase(0, 1);
    }
    
    if (!arg.empty()) {
      for (const Option &opt : options) {
        std::string_view opt_name(opt.name);
        
        if (!(opt.context & cmd)) continue;
          
        if (arg.rfind(opt_name, 0) != 0) {
          if (arg.at(0) != opt.short_name) continue;
          else {
            opt_name = std::string(1, opt.short_name);
          }
        }

        if (opt.requires_arg) {
          std::string opt_arg;
          // Connected argument.
          if (arg.length() > opt_name.length()) {
            opt_arg = arg.substr(opt_name.length());
            if (opt_arg.at(0) == '=') {
              opt_arg.erase(0, 1);
            }
            if (opt_arg.length()) {
              valid_opt = true;
            }
          }
          // Seperated argument.
          else {
            if (args.size() > arg_index + 1) {
              opt_arg = args.at(arg_index + 1);
              arg_index++;
              valid_opt = true;
            }
          }
          if (valid_opt) {
            input_opts.push_back({ opt, opt_arg });
          }
          else {
            log_err << ARGS_ERR_TAG << LogStyle::BOLD << "Missing argument for option `" << "-" << opt.name << "'\n" << LogStyle::CLEAR;
            should_exit = true;
            exit_code = ExitCode::INVALID_ARGS;
            return;
          }
          break;
        }
        else {
          if (arg.length() != opt_name.length()) continue;
          
          valid_opt = true;
          input_opts.push_back({ opt, arg });
          break;
        }
      }
    }
    if (!valid_opt) {
      log_err << ARGS_ERR_TAG << LogStyle::BOLD << "Invalid option `" << "-" << arg << "'\n" << LogStyle::CLEAR;
      should_exit = true;
      exit_code = ExitCode::INVALID_ARGS;
      return;
    }
  }

  auto invalid_arg = [&](Logger& log, const InputOption& opt) {
    log << ARGS_ERR_TAG << LogStyle::BOLD << "Invalid argument `" << opt.arg << "' for option `-" << opt.option.name << "'\n" << LogStyle::CLEAR;
    exit_code = ExitCode::INVALID_ARGS;
  };

  for (const InputOption &opt : input_opts) {
    switch (opt.option.index) {
      case OPT_EMIT_ASSEMBLY:   emit_asm = true; break;
      case OPT_EMIT_IRGEN:      emit_irgen = true; break;
      case OPT_EMIT_IR:         emit_ir = true; break;
      case OPT_EMIT_BC:         emit_bc = true; break;
      case OPT_EMIT_OBJECT:     emit_obj = true; break;
      case OPT_EMIT_EXECUTABLE: emit_exe = true; break;
      case OPT_EMIT_LIBRARY:    emit_lib = true; break;
      case OPT_OS:
        if      (opt.arg == "windows" || opt.arg == "win32") os = OS::WINDOWS;
        else if (opt.arg == "darwin"  || opt.arg == "macOS") os = OS::MACOS;
        else if (opt.arg == "linux") os = OS::LINUX;
        else if (opt.arg == "none")  os = OS::NONE;
        else invalid_arg(log_err, opt);
        break;
      case OPT_ARCH:
        if      (opt.arg == "x86-64" || opt.arg == "x86_64"  || opt.arg == "amd64") arch = Arch::X86_64;
        else if (opt.arg == "i686"   || opt.arg == "x86") arch = Arch::I686;
        else if (opt.arg == "arm64"  || opt.arg == "aarch64" || opt.arg == "armv8") arch = Arch::AARCH64;
        else if (opt.arg == "armv6")  arch = Arch::ARMV6;
        else if (opt.arg == "armv7")  arch = Arch::ARMV7;
        else if (opt.arg == "wasm32") arch = Arch::WASM32;
        else if (opt.arg == "wasm64") arch = Arch::WASM64;
        else invalid_arg(log_err, opt);
        break;
      case OPT_PRINT_AST:    print_ast = true; break;
      case OPT_VERBOSE:      verbose_output = true; break;
      case OPT_DEBUG:        debug_symbols = true; break;
      case OPT_JOB_NUM:
        try {
          s32 j = std::stoi(opt.arg);
          job_num = j > 0 ? j : 0;
        } catch (std::invalid_argument&) {
          job_num = 0;
        }
        if (job_num < 1) invalid_arg(log_err, opt);
        break;
      case OPT_LIBRARY_PATH: lib_paths.push_back(opt.arg); break;
      case OPT_LINK_LIBRARY: link_libs.push_back(opt.arg); break;
      case OPT_OPTIMIZE:     optimize = true; break;
      case OPT_OUTPUT:       output_path = opt.arg; break;
      case OPT_WNO:          no_warnings = true; break;
      case OPT_WERROR:       error_warnings = true; break;
    }
  }

  if (verbose_output) {
    log_out << LogStyle::BOLD << "Compiler invocation: " << LogStyle::CLEAR;
    for (const std::string &arg : args) {
      log_out << arg << " ";
    }
    log_out << '\n';
  }
}