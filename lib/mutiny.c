#include <mutiny/mutiny.h>
#include <mutiny/exit_code.h>
#include <mutiny/settings.h>
#include <mutiny/args.h>
#include <mutiny/translation_unit.h>
#include <mutiny/util/timer.h>
#include <mutiny/util/log.h>

int main(int argc, char* const* argv) {
  mt_log_t verbose_log = mt_log_init(stdout, MT_VERBOSE);
  mt_log_t err_log = mt_log_init(stderr, MT_ERROR);
  mt_timer_t prog_timer = mt_timer_init();
  
  mt_settings_t* settings = NULL;
  mt_translation_unit_t* t_unit = NULL;
  
  int exit_code = MT_EXIT_SUCCEESS;
  
  do {
    // --- Command-Line Argument Parsing ---
    {
      mt_timer_t arg_timer = mt_timer_init();
      
      settings = mt_args_decode(argc, argv);
      
      if (settings && settings->verbose) {
        mt_log_add(&verbose_log, "Argument parser stage finished in %.2fs\n", mt_timer_get(&arg_timer));
        mt_log_dump(&verbose_log);
      }
      
      if (!settings || settings->exit_code) {
        mt_log_add(&err_log, "Argument parser failed. Exiting now.\n");
        mt_log_dump(&err_log);
        exit_code = settings->exit_code ? settings->exit_code : MT_EXIT_ERR_INTERNAL;
        break;
      }
      if (settings->end) {
        break;
      }
    }
    
    // --- Initialize Translation Unit ---
    {
      t_unit = mt_translation_unit_init(settings);
      if (!t_unit || settings->exit_code) {
        mt_log_add(&err_log, "Translation unit failed to initialize. Exiting now.\n");
        mt_log_dump(&err_log);
        break;
      }
    }
    
    // --- Execute the Parser ---
    {
      mt_timer_t parser_timer = mt_timer_init();
      
      bool parser_res = mt_translation_unit_parse_exec(t_unit);
      
      if (settings->verbose) {
        mt_log_add(&verbose_log, "Parser stage finished in %.2fs\n", mt_timer_get(&parser_timer));
        mt_log_dump(&verbose_log);
      }
      
      if (!parser_res || settings->exit_code) {
        mt_log_add(&err_log, "Parser failed. Exiting now.\n");
        mt_log_dump(&err_log);
        exit_code = settings->exit_code ? settings->exit_code : MT_EXIT_ERR_INTERNAL;
        break;
      }
      if (settings->end || settings->type == MT_COMPILER_PARSER) {
        break;
      }
    }
  } while (0);
  
  if (settings && settings->verbose) {
    mt_log_add(&verbose_log, "Compiler finished in %.2fs\n", mt_timer_get(&prog_timer));
    mt_log_dump(&verbose_log);
  }
  
  mt_translation_unit_deinit(t_unit);
  mt_settings_deinit(settings);
  
  return exit_code;
}
