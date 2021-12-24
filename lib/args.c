#include <mutiny/mutiny.h>
#include <mutiny/args.h>
#include <mutiny/settings.h>
#include <mutiny/error/error.h>
#include <mutiny/util/list.h>
#include <getopt.h>

typedef enum _mt_command {
  cmd_unknown = 0,
  cmd_run,
  cmd_build,
  cmd_version,
  cmd_help,
} mt_command_t;

static void usage(FILE* f);
static void version(void);
static mt_command_t parse_command(const char* cmd);

mt_settings_t* decode_args(unsigned argc, char* const* argv) {
  mt_settings_t* s = malloc(sizeof(mt_settings_t));
  memset(s, 0, sizeof(mt_settings_t));
  
  if (argc < 2) {
    printf("Mutiny is a tool for managing mutiny source code\n\n");
    usage(stdout); s->end = true; return s; }
  
  mt_command_t cmd = parse_command(argv[1]);
  
  switch(cmd) {
    case cmd_run:
      s->type = JIT;
      break;
    case cmd_build:
      s->type = COMPILER;
      break;
    case cmd_version:
      version();
      s->end = true;
      return s;
    case cmd_help:
      usage(stdout);
      s->end = true;
      return s;
    case cmd_unknown:
      fprintf(stderr, MT_ERROR "unknown command `%s'\n\n", argv[1]);
      usage(stderr);
      s->exit_code = exit_err_args;
      return s;
  }
  
  // --- Parsing ---
  
  const char* short_opts = "OL:wvo:";
  
  enum {
    OPT_HIDE_WARNINGS = 0,
    OPT_PRINT_AST,
    OPT_VERBOSE,
    OPT_OS,
    OPT_ARCH,
    OPT_EMIT,
  };
  
  const struct option long_opts[] = {
    [OPT_HIDE_WARNINGS] = { "hide-warnings", no_argument, 0, 'w' },
    [OPT_PRINT_AST]     = { "print-ast",     no_argument, 0,  0  },
    [OPT_VERBOSE]       = { "verbose" ,      no_argument, 0, 'v' },
    
    // JIT options.
    // ...
    
    // Build options.
    
    [OPT_OS]   = { "os",   required_argument, 0,  0  },
    [OPT_ARCH] = { "arch", required_argument, 0,  0  },
    [OPT_EMIT] = { "emit", required_argument, 0,  0  },
    
    { NULL, 0, 0, 0 }
  };
  
  opterr = false;
  
#define OPT_TYPE(t) \
    if (s->type != type) goto UNRECOGNIZED_OPTION

#define OPT_ARG_TYPE(t) \
    if (s->type != t) { \
      --optind; \
      goto UNRECOGNIZED_OPTION; \
    }
  
  int opt, i;
  const char* opt_name;
  // TODO Write custom option parser for greater portability.
  while ((opt = getopt_long_only(argc, argv, short_opts, long_opts, &i)) != -1) {
    switch (opt) {
      case 0:
        opt_name = long_opts[i].name;
        switch (i) {
          case OPT_PRINT_AST:
            break;
          // Target operating system.
          case OPT_OS:
            OPT_ARG_TYPE(COMPILER);
            if (s->os) goto DUPLICATE_OPTION;
            if      (!strcmp(optarg, "windows") || !strcmp(optarg, "win32")) s->os = OS_WINDOWS;
            else if (!strcmp(optarg, "darwin")  || !strcmp(optarg, "macOS")) s->os = OS_DARWIN;
            else if (!strcmp(optarg, "linux")) s->os = OS_LINUX;
            else if (!strcmp(optarg, "none"))  s->os = OS_NONE;
            else goto INVALID_ARGUMENT;
            break;
          // Target architecture.
          case OPT_ARCH:
            OPT_ARG_TYPE(COMPILER);
            if (s->arch) goto DUPLICATE_OPTION;
            if      (!strcmp(optarg, "x86-64") || !strcmp(optarg, "x86_64")  || !strcmp(optarg, "amd64")) s->arch = ARCH_X86_64;
            else if (!strcmp(optarg, "i686")   || !strcmp(optarg, "x86")) s->arch = ARCH_I686;
            else if (!strcmp(optarg, "arm64")  || !strcmp(optarg, "aarch64") || !strcmp(optarg, "armv8")) s->arch = ARCH_AARCH64;
            else if (!strcmp(optarg, "armv6"))  s->arch = ARCH_ARMV6;
            else if (!strcmp(optarg, "armv7"))  s->arch = ARCH_ARMV7;
            else if (!strcmp(optarg, "wasm32")) s->arch = ARCH_WASM32;
            else if (!strcmp(optarg, "wasm64")) s->arch = ARCH_WASM64;
            else goto INVALID_ARGUMENT;
            break;
          // Types of output.
          case OPT_EMIT:
            OPT_ARG_TYPE(COMPILER);
            // TODO Type of output for the compiler to emit.
            break;
        }
        break;
      case 'O':
        s->optimize = true;
        break;
      case 'L':
        l_push(s->lib_dirs, char*, strdup(optarg));
        break;
      case 'w':
        s->hide_warnings = true;
        break;
      case 'v':
        s->verbose = true;
        break;
      case 'o':
        OPT_ARG_TYPE(COMPILER);
        if (s->output) goto DUPLICATE_OPTION;
        s->output = strdup(optarg);
        break;
UNRECOGNIZED_OPTION:
      case '?':
        fprintf(stderr, MT_ERROR "unrecognized option ");
        if (opt && opt != '?') {
          fprintf(stderr, "`-%c'", opt);
        }
        else {
          fprintf(stderr, "`%s'", argv[optind - 1]);
        }
        fprintf(stderr, "\n");
        s->exit_code = exit_err_args;
        return s;
INVALID_ARGUMENT:
        fprintf(stderr, MT_ERROR "invalid argument ");
        if (optarg) {
          fprintf(stderr, "`%s' ", optarg);
        }
        fprintf(stderr, "to option ");
        if (!opt) {
          fprintf(stderr, "`--%s'", opt_name);
        }
        else if (opt != '?') {
          fprintf(stderr, "`-%c'", opt);
        }
        fprintf(stderr, "\n");
        s->exit_code = exit_err_args;
        return s;
DUPLICATE_OPTION:
        fprintf(stderr, MT_ERROR "duplicate option ");
        if (!opt) {
          fprintf(stderr, "`--%s'", opt_name);
        }
        else if (opt != '?') {
          fprintf(stderr, "`-%c'", opt);
        }
        else {
          fprintf(stderr, "`%s'", argv[optind - 1]);
        }
        fprintf(stderr, "\n");
        s->exit_code = exit_err_args;
        return s;
    }
  }
  
  while (optind < argc) {
    l_push(s->src_dirs, char*, strdup(argv[optind++]));
  }
  
  return s;
}

static void usage(FILE* f) {
  fprintf(f,
    "Usage:\n"
    "\tmutiny <command> [arguments]\n"
    "\n"
    // TODO Usage.
  );
}

static void version(void) {

}

static mt_command_t parse_command(const char* cmd) {
  if(!strcmp(cmd, "run"))           { return cmd_run; }
  else if (!strcmp(cmd, "build"))   { return cmd_build; }
  else if (!strcmp(cmd, "version")) { return cmd_version; }
  else if (!strcmp(cmd, "help"))    { return cmd_help; }
  else                              { return cmd_unknown; }
}
