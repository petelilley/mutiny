#include <mutiny/mutiny.h>
#include <mutiny/exit_code.h>
#include <mutiny/settings.h>
#include <mutiny/args.h>
#include <mutiny/util/timer.h>

int main(int argc, char* const* argv) {
  mt_settings_t* s;
  
  BEGIN_TIMER(prog_time);
  
  // --- Input arguments ---
  
  BEGIN_TIMER(args_time);
  
  // Parse and interpert the input arguments.
  s = decode_args(argc, argv);
  
  END_TIMER(args_time);
  if (s->verbose) printf("parsed input arguments in %fs\n", args_time);
  if (s->end || s->exit_code) goto CLEANUP;
  
  END_TIMER(prog_time);
  
CLEANUP:
  // TODO Free settings.
  
  return s->exit_code;
}
