#include <mutiny/mutiny.h>
#include <mutiny/args.h>
#include <mutiny/settings.h>
#include <mutiny/util/list.h>
#include <mutiny/util/log.h>
#include <getopt.h>

typedef enum _mt_command {
  CMD_UNKNOWN = 0,
  CMD_RUN,
  CMD_BUILD,
  CMD_PARSE,
  CMD_VERSION,
  CMD_HELP,
} mt_command_t;

typedef struct _mt_option {
  const char* const name;
  const char short_name;
  const bool req_arg;
  const mt_compiler_type_t type;
  const char* const help;
} mt_option_t;

typedef struct _mt_input_option {
  ssize_t index;
  char* arg;
  char* file_path;
  bool error;
} mt_input_option_t;
  
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
  OPT_IMPORT_PATH,
  OPT_JOB_NUM,
  OPT_LIBRARY_PATH,
  OPT_LINK_LIBRARY,
  OPT_OPTIMIZE,
  OPT_OUTPUT,
  OPT_NO_WARNINGS,
};

static const mt_option_t opts[] = {
  [OPT_EMIT_ASSEMBLY]    = { "emit-assembly",   'S', false, MT_COMPILER_COMPILER, "Emit assembly file(s) (-S)"                         },
  [OPT_EMIT_IRGEN]       = { "emit-irgen",       0 , false, MT_COMPILER_COMPILER, "Emit LLVM IR file(s) before LLVM optimizations"     },
  [OPT_EMIT_IR]          = { "emit-ir",          0 , false, MT_COMPILER_COMPILER, "Emit LLVM IR file(s) after LLVM optimizations"      },
  [OPT_EMIT_BC]          = { "emit-bc",          0 , false, MT_COMPILER_COMPILER, "Emit LLVM BC file(s)"                               },
  [OPT_EMIT_OBJECT]      = { "emit-object",     'c', false, MT_COMPILER_COMPILER, "Emit object file(s) (-c)"                           },
  [OPT_EMIT_EXECUTABLE]  = { "emit-executable",  0 , false, MT_COMPILER_COMPILER, "Emit a linked executable"                           },
  [OPT_EMIT_LIBRARY]     = { "emit-library",     0 , false, MT_COMPILER_COMPILER, "Emit a linked library"                              },
  
  [OPT_OS]               = { "os",               0,  true,  MT_COMPILER_COMPILER, "Specify the target operating system"                },
  [OPT_ARCH]             = { "arch",             0,  true,  MT_COMPILER_COMPILER, "Specify the target architecture"                    },
  
  [OPT_PRINT_AST]        = { "print-ast",        0 , false, MT_COMPILER_ANY,      "Print generated AST(s)"                             },
  
  [OPT_VERBOSE]          = { NULL,              'v', false, MT_COMPILER_ANY,      "Use verbose output"                                 },
  [OPT_DEBUG]            = { NULL,              'g', false, MT_COMPILER_ANY,      "Emit debug symbols"                                 },
  [OPT_IMPORT_PATH]      = { NULL,              'I', true , MT_COMPILER_ANY,      "Add path to the import search path"                 },
  [OPT_JOB_NUM]          = { NULL,              'j', true , MT_COMPILER_ANY,      "Max number of jobs to execute in parallel"          },
  [OPT_LIBRARY_PATH]     = { NULL,              'L', true , MT_COMPILER_ANY,      "Add path to library link search path"               },
  [OPT_LINK_LIBRARY]     = { NULL,              'l', true , MT_COMPILER_ANY,      "Specifies a library which should be linked against" },
  [OPT_OPTIMIZE]         = { NULL,              'O', false, MT_COMPILER_ANY,      "Compile with optimizations"                         },
  [OPT_OUTPUT]           = { NULL,              'o', true , MT_COMPILER_COMPILER, "Specify file to write output to"                    },
  [OPT_NO_WARNINGS]      = { NULL,              'w', false, MT_COMPILER_ANY,      "Hide all compiler warnings"                         },
};

#define OPT_NUM (sizeof(opts) / sizeof(mt_option_t))

static void usage(mt_log_t* log, mt_command_t section);
static void version(void);
static mt_command_t parse_command(const char* cmd);

mt_input_option_t next_opt(unsigned argc, char* const* argv, mt_compiler_type_t type, size_t* i, mt_log_t* err_log);

mt_settings_t* mt_args_decode(unsigned argc, char* const* argv) {
  mt_settings_t* s = mt_settings_init();
  s->stage = MT_STAGE_ARGS;
  
  mt_log_t help_log = mt_log_init(stderr, "");
  mt_log_t err_log = mt_log_init(stderr, MT_ERROR);
  
  if (argc < 2) {
    mt_log_add(&help_log, "Mutiny is a tool for managing mutiny source code\n\n");
    mt_log_dump(&help_log);
    usage(&help_log, CMD_UNKNOWN);
    s->end = true;
    return s;
  }
  
  mt_command_t cmd = parse_command(argv[1]);
  
  switch(cmd) {
    case CMD_RUN:
      s->type = MT_COMPILER_JIT;
      break;
    case CMD_BUILD:
      s->type = MT_COMPILER_COMPILER;
      break;
    case CMD_PARSE:
      s->type = MT_COMPILER_PARSER;
      break;
    case CMD_VERSION:
      version();
      s->end = true;
      return s;
    case CMD_HELP:
      // TODO Help for specific command.
      s->end = true;
      if (argc > 2) {
        usage(&help_log, parse_command(argv[2]));
      } else {
        usage(&help_log, CMD_UNKNOWN);
      }
      return s;
    case CMD_UNKNOWN:
      mt_log_add(&err_log, "Unknown command `%s'\n\n", argv[1]);
      mt_log_dump(&err_log);
      usage(&err_log, CMD_UNKNOWN);
      s->exit_code = MT_EXIT_ERR_ARGS;
      return s;
  }
  
  // --- Parsing ---
  
  size_t i = 0;
  const char* opt_name;
  size_t opt_len;
  mt_input_option_t opt;
  while (true) {
    if ((opt = next_opt(argc - 1, argv + 2, s->type, &i, &err_log)).index == -1) {
      if (opt.file_path) {
        l_push(s->src_dirs, opt.file_path);
        continue;
      }
      else {
        if (opt.error) {
          s->exit_code = MT_EXIT_ERR_ARGS;
        }
        break;
      }
    }
    
    if (opts[opt.index].name) {
      opt_name = opts[opt.index].name;
      opt_len = strlen(opt_name);
    }
    else {
      opt_name = &opts[opt.index].short_name;
      opt_len = 1;
    }
    
    switch (opt.index) {
      case OPT_EMIT_ASSEMBLY:
        s->emit_asm = true;
        break;
      case OPT_EMIT_IRGEN:
        s->emit_irgen = true;
        break;
      case OPT_EMIT_IR:
        s->emit_ir = true;
        break;
      case OPT_EMIT_BC:
        s->emit_bc = true;
        break;
      case OPT_EMIT_OBJECT:
        s->emit_obj = true;
        break;
      case OPT_EMIT_EXECUTABLE:
        s->emit_exe = true;
        break;
      case OPT_EMIT_LIBRARY:
        s->emit_lib = true;
        break;
      case OPT_OS:
        if (s->os) goto DUPLICATE_OPTION;
        if      (!strcmp(opt.arg, "windows") || !strcmp(opt.arg, "win32")) s->os = MT_OS_WINDOWS;
        else if (!strcmp(opt.arg, "darwin")  || !strcmp(opt.arg, "macOS")) s->os = MT_OS_DARWIN;
        else if (!strcmp(opt.arg, "linux")) s->os = MT_OS_LINUX;
        else if (!strcmp(opt.arg, "none"))  s->os = MT_OS_NONE;
        else goto INVALID_ARGUMENT;
        free(opt.arg);
        break;
      case OPT_ARCH:
        if (s->arch) goto DUPLICATE_OPTION;
        if      (!strcmp(opt.arg, "x86-64") || !strcmp(opt.arg, "x86_64")  || !strcmp(opt.arg, "amd64")) s->arch = MT_ARCH_X86_64;
        else if (!strcmp(opt.arg, "i686")   || !strcmp(opt.arg, "x86")) s->arch = MT_ARCH_I686;
        else if (!strcmp(opt.arg, "arm64")  || !strcmp(opt.arg, "aarch64") || !strcmp(opt.arg, "armv8")) s->arch = MT_ARCH_AARCH64;
        else if (!strcmp(opt.arg, "armv6"))  s->arch = MT_ARCH_ARMV6;
        else if (!strcmp(opt.arg, "armv7"))  s->arch = MT_ARCH_ARMV7;
        else if (!strcmp(opt.arg, "wasm32")) s->arch = MT_ARCH_WASM32;
        else if (!strcmp(opt.arg, "wasm64")) s->arch = MT_ARCH_WASM64;
        else goto INVALID_ARGUMENT;
        free(opt.arg);
        break;
      case OPT_PRINT_AST:
        s->print_ast = true;
        break;
      case OPT_VERBOSE:
        s->verbose = true;
        break;
      case OPT_DEBUG:
        s->debug = true;
        break;
      case OPT_IMPORT_PATH:
        l_push(s->import_paths, opt.arg);
        break;
      case OPT_JOB_NUM:
        if ((s->job_num = (unsigned)atoi(opt.arg)) <= 0) {
          goto INVALID_ARGUMENT;
        }
        free(opt.arg);
        break;
      case OPT_LIBRARY_PATH:
        l_push(s->lib_paths, opt.arg);
        break;
      case OPT_LINK_LIBRARY:
        l_push(s->link_libs, opt.arg);
        break;
      case OPT_OPTIMIZE:
        s->optimize = true;
        break;
      case OPT_OUTPUT:
        if (s->output) goto DUPLICATE_OPTION;
        s->output = opt.arg;
        break;
      case OPT_NO_WARNINGS:
        s->no_warnings = true;
        break;
    }
  }
  
  return s;
  
INVALID_ARGUMENT:
  mt_log_add(&err_log, "Invalid argument `%s' to option `%.*s'\n", opt.arg, (int)opt_len, opt_name);
  mt_log_dump(&err_log);
  s->exit_code = MT_EXIT_ERR_ARGS;
  free(opt.arg);
  return s;
  
DUPLICATE_OPTION:
  mt_log_add(&err_log, "Duplicate option `%.*s'\n", (int)opt_len, opt_name);
  mt_log_dump(&err_log);
  s->exit_code = MT_EXIT_ERR_ARGS;
  if (opt.arg) free(opt.arg);
  return s;
}

mt_input_option_t next_opt(unsigned argc, char* const* argv, mt_compiler_type_t type, size_t* i, mt_log_t* err_log) {
  mt_input_option_t opt = {-1, NULL, NULL, false};
  
  if (*i + 1 >= argc) {
    return opt;
  }
  
  const char* arg = argv[*i];
  // Get rid of leading '-' or '--'.
  if (*arg == '-') {
    ++arg;
    if (*arg && *arg == '-') {
      ++arg;
    }
  }
  // If no '-', then it's a path to a source file.
  else {
    opt.file_path = strdup(arg);
    ++*i;
    return opt;
  }
  
  size_t arg_len = strlen(arg);
  
  // Compare against each possible option.
  for (size_t n = 0; n < OPT_NUM; ++n) {
    const mt_option_t* o = &opts[n];
    
    // Check if a type was given.
    if (o->type != MT_COMPILER_ANY && o->type != type) {
      continue;
    }
    
    // No argument or seperated argument.
    if ((o->name && !strcmp(arg, o->name)) ||
        (arg_len == 1 && o->short_name && !strncmp(arg, &o->short_name, 1))) {
      if (o->req_arg) {
        // No more arguments given.
        if (*i + 2 == argc) {
          goto MISSING_ARGUMENT;
        }
        opt.arg = strdup(argv[++*i]);
      }
      goto FOUND;
    }
    // Connected argument.
    else if (o->req_arg) {
      size_t o_len = o->name ? strlen(o->name) : o->short_name != 0;
      if ((o->name && !strncmp(arg, o->name, o_len)) || (o->short_name && !strncmp(arg, &o->short_name, 1))) {
        // An equals sign to seperate option from argument.
        if (arg[o_len] == '=') {
          // If there is no argument after the '='.
          if (!*(arg + o_len + 1)) {
            goto MISSING_ARGUMENT;
          }
          arg++;
        }
        opt.arg = strdup(arg + o_len);
        goto FOUND;
      }
    }
    // Not found, so continue to next option.
    continue;
    
FOUND:
    opt.index = (ssize_t)n;
    ++*i;
    return opt;
  }
  
// UNRECOGNIZED_OPTION:
  mt_log_add(err_log, "Unrecognized option `%s'\n", argv[*i]);
  mt_log_dump(err_log);
  goto ERROR;
  
MISSING_ARGUMENT:
  mt_log_add(err_log, "Missing argument to option `%s'\n", argv[*i]);
  mt_log_dump(err_log);
  goto ERROR;
  
ERROR:
  opt.error = true;
  ++*i;
  return opt;
}

static void print_options(FILE* f, mt_compiler_type_t type) {
  for (unsigned i = 0; i < OPT_NUM; ++i) {
    const mt_option_t* opt = &opts[i];
    if (opt->type == MT_COMPILER_ANY || opt->type == type) {
      if (opt->name) {
        fprintf(f, "\t-%s", opt->name);
        size_t len = strlen(opt->name);
        if (len < 15) {
          fprintf(f, "\t");
        }
        if (len < 7) {
          fprintf(f, "\t");
        }
      }
      else {
        fprintf(f, "\t-%c\t\t", opt->short_name);
      }
      fprintf(f, "%s\n", opt->help);
    }
  }
}

static void usage(mt_log_t* l, mt_command_t section) {
  switch (section) {
    case CMD_RUN:
      fprintf(l->file,
        "Usage: mutiny run [sources] [flags]\n"
        "\n"
        "Available options:\n"
      );
      print_options(l->file, MT_COMPILER_JIT);
      break;
    case CMD_BUILD:
      fprintf(l->file,
        "Usage: mutiny build [-o output] [sources] [flags]\n"
        "\n"
        "Available options:\n"
      );
      print_options(l->file, MT_COMPILER_COMPILER);
      break;
    case CMD_PARSE:
      fprintf(l->file,
        "Usage: mutiny parse [sources] [flags]\n"
        "\n"
        "Available options:\n"
      );
      print_options(l->file, MT_COMPILER_PARSER);
      break;
    default:
      fprintf(l->file,
        "Usage: mutiny <command> [arguments]\n"
        "\n"
        "Available commands:\n"
        "\trun\t\tCompile and run a Mutiny program\n"
        "\tbuild\t\tCompile a Mutiny program\n"
        "\tparse\t\tParse a Mutiny program\n"
        "\tversion\t\tPrint the compiler version and exit\n"
        "\thelp\t\tPrint this usage information and exit\n"
        "\n"
        "Run `mutiny help <command>' for information about a specific command.\n"
      );
      break;
  }
}

static void version(void) {
  printf("mutiny version %s %s/%s\n", MT_VERSION_FULL_STR, MT_HOST_OS_STR, MT_HOST_ARCH_STR);
}

static mt_command_t parse_command(const char* cmd) {
  if (!strcmp(cmd, "run")        || !strcmp(cmd, "jit"))     { return CMD_RUN;     }
  else if (!strcmp(cmd, "build") || !strcmp(cmd, "compile")) { return CMD_BUILD;   }
  else if (!strcmp(cmd, "parse"))                            { return CMD_PARSE;   }
  else if (!strcmp(cmd, "version"))                          { return CMD_VERSION; }
  else if (!strcmp(cmd, "help"))                             { return CMD_HELP;    }
  else                                                       { return CMD_UNKNOWN; }
}
