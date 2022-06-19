#include <mutiny/mutiny.h>
#include <mutiny/translation_unit/parser/parser.h>
#include <mutiny/translation_unit/translation_unit.h>
#include <mutiny/translation_unit/lexer/token.h>
#include <mutiny/translation_unit/lexer/lexer.h>
#include <mutiny/ast/ast.h>
#include <mutiny/settings.h>
#include <mutiny/util/list.h>
#include <mutiny/util/log.h>
#include <mutiny/util/filesystem.h>

mt_translation_unit_t* mt_translation_unit_init(struct _mt_file* file, struct _mt_settings* settings) {
  mt_translation_unit_t* tu = malloc(sizeof(mt_translation_unit_t));
  tu->settings = settings;
  tu->file = file;
  tu->tokens = NULL;
  tu->ast = NULL;
  tu->error_reporter = mt_error_reporter_init();
  return tu;
}

void mt_translation_unit_deinit(mt_translation_unit_t* tu) {
  if (!tu) return;
  
  if (tu->tokens) {
    mt_token_deinit(tu->tokens);
  }
  free(tu);
}

bool mt_translation_unit_parse_exec(mt_translation_unit_t* tu) {
  mt_translation_unit_tokenize(tu);
  if (tu->error_reporter.warn_num > 0) {
    mt_log_dump(&tu->error_reporter.warn_log);
  }
  if (tu->error_reporter.err_num > 0) {
    mt_log_dump(&tu->error_reporter.err_log);
    return false;
  }
  
  mt_translation_unit_parse_tokens(tu);
  if (tu->error_reporter.warn_num > 0) {
    mt_log_dump(&tu->error_reporter.warn_log);
  }
  if (tu->error_reporter.err_num > 0) {
    mt_log_dump(&tu->error_reporter.err_log);
    return false;
  }
  
  // TODO Concatenate ast.

  return true;
}
