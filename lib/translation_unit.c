#include <mutiny/mutiny.h>
#include <mutiny/parser/parser.h>
#include <mutiny/translation_unit.h>
#include <mutiny/parser/token.h>
#include <mutiny/parser/lexer.h>
#include <mutiny/parser/parser_util.h>
#include <mutiny/ast/ast.h>
#include <mutiny/settings.h>
#include <mutiny/error/error.h>
#include <mutiny/util/list.h>
#include <mutiny/util/filesystem.h>

mt_translation_unit_t* translation_unit_init(struct _mt_settings* s) {
  s->stage = STAGE_PARSE;
  
  size_t src_num = l_size(s->src_dirs);
  if (!src_num) {
    fprintf(stderr, MT_ERROR "no input files\n");
    s->exit_code = EXIT_ERR_FILE;
    return NULL;
  }
  
  mt_translation_unit_t* tu = malloc(sizeof(mt_translation_unit_t));
  tu->settings = s;
  l_init(tu->files);
  tu->tokens = NULL;
  tu->ast = NULL;
  
  const char* path;
  mt_file_t* f = NULL;
  
  for (size_t i = 0; i < src_num; ++i) {
    path = l_at(s->src_dirs, i);
    
    if (is_directory(path)) {
      // TODO Something.
    }
    
    f = file_open(path);
    if (!f) {
      s->exit_code = EXIT_ERR_FILE;
      break;
    }
    
    l_push(tu->files, mt_file_t*, f);
  }
  
  if (s->exit_code == EXIT_ERR_FILE) {
    translation_unit_deinit(tu);
    return NULL;
  }
  
  return tu;
}

void translation_unit_deinit(mt_translation_unit_t* tu) {
  if (!tu) return;
  
  for (size_t i = 0; i < l_size(tu->files); i++) {
    file_deinit(l_at(tu->files, i));
  }
  l_deinit(tu->files);
  if (tu->tokens) {
    // TODO Deinit tokens.
  }
  if (tu->ast) {
    // TODO Deinit ast.
  }
  free(tu);
}

bool translation_unit_parse_exec(mt_translation_unit_t* tu) {
  // TODO Use thread pool to parse multiple source files simultaneously.
  for (size_t i = 0; i < l_size(tu->files); ++i) {
    tu->tokens = tokenize(l_at(tu->files, i), tu->settings);
    if (!tu->tokens) {
      return false;
    }
    
    tu->ast = parse_tokens(tu->tokens, tu->settings);
    if (!tu->ast) {
      return false;
    }
    
    // TODO Concatenate ast.
  }

  return true;
}

bool translation_unit_semantic_analysis_exec(mt_translation_unit_t* tu) {
  return false;
}

bool translation_unit_irgen_exec(mt_translation_unit_t* tu) {
  return false;
}
