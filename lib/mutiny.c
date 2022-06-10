#include <mutiny/mutiny.h>
#include <mutiny/exit_code.h>
#include <mutiny/settings.h>
#include <mutiny/args.h>
#include <mutiny/translation_unit.h>
#include <mutiny/util/timer.h>

int main(int argc, char* const* argv) {
  mt_settings_t* s = NULL;
  mt_translation_unit_t* tu = NULL;
  
  BEGIN_TIMER(prog_time);
  
  // --- Input arguments ---
  
  BEGIN_TIMER(args_time);
  
  // Parse and interpert the input arguments.
  s = decode_args(argc, argv);
  
  END_TIMER(args_time);
  if (s->verbose) printf("parsed input arguments in %fs\n", args_time);
  if (s->end || s->exit_code) goto CLEANUP;
  
  BEGIN_TIMER(parse_time);
  
  tu = translation_unit_init(s);
  
  translation_unit_exec(tu);
  
  END_TIMER(parse_time);
  if (s->verbose) printf("parsed source files in %fs\n", parse_time);
  if (s->end || s->exit_code) goto CLEANUP;
  
CLEANUP:
  // TODO Free settings.
  
  END_TIMER(prog_time);
  if (s->verbose) printf("compiler finished in %fs\n", prog_time);
  
  return s->exit_code;
}
