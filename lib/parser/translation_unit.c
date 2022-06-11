#include <mutiny/mutiny.h>
#include <mutiny/parser/parser.h>
#include <mutiny/parser/translation_unit.h>
#include <mutiny/parser/token.h>
#include <mutiny/parser/lexer.h>
#include <mutiny/parser/parser_util.h>
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
  return tu;
}

void mt_translation_unit_deinit(mt_translation_unit_t* tu) {
  if (!tu) return;
  
  if (tu->tokens) {
    // TODO Deinit tokens.
  }
  if (tu->ast) {
    // TODO Deinit ast.
  }
  free(tu);
}

bool mt_translation_unit_parse_exec(mt_translation_unit_t* tu) {
  tu->tokens = mt_tokenize(tu->file, tu->settings);
  if (!tu->tokens) {
    return false;
  }
  
  tu->ast = mt_parse_tokens(tu->tokens, tu->settings);
  if (!tu->ast) {
    return false;
  }
  
  // TODO Concatenate ast.

  return true;
}
