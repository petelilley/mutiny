#include <mutiny/mutiny.h>
#include <mutiny/parser/parser.h>
#include <mutiny/compiler.h>
#include <mutiny/parser/token.h>
#include <mutiny/parser/lexer.h>
#include <mutiny/parser/parser_util.h>
#include <mutiny/parser/translation_unit.h>
#include <mutiny/ast/ast.h>
#include <mutiny/settings.h>
#include <mutiny/util/list.h>
#include <mutiny/util/log.h>
#include <mutiny/util/filesystem.h>

mt_compiler_t* mt_compiler_init(struct _mt_settings* s) {
  mt_log_t err_log = mt_log_init(stderr, MT_ERROR);
  mt_log_t warn_log = mt_log_init(stderr, MT_WARNING);
  
  size_t src_num = l_size(s->src_dirs);
  if (!src_num) {
    mt_log_add(&err_log, "No input files\n");
    mt_log_dump(&err_log);
    s->exit_code = MT_EXIT_ERR_FILE;
    return NULL;
  }
  
  mt_compiler_t* c = malloc(sizeof(mt_compiler_t));
  c->settings = s;
  l_init(c->files);
  l_init(c->t_unit);
  c->ast = NULL;
  
  const char* path;
  mt_file_t* f = NULL;
  
  for (size_t i = 0; i < src_num; ++i) {
    path = l_at(s->src_dirs, i);
    
    if (is_directory(path)) {
      // TODO Something.
    }
    
    f = mt_file_init(path);
    if (!f) {
      s->exit_code = MT_EXIT_ERR_FILE;
      break;
    }
    
    if (!END(path, ".mutiny") && !s->no_warnings) {
      mt_log_add(&warn_log, "Filename extension does not match '.mutiny' but will be treated as a mutiny source file.\n", path);
      mt_log_dump(&warn_log);
    }
    
    l_push(c->files, mt_file_t*, f);
  }
  
  if (s->exit_code == MT_EXIT_ERR_FILE) {
    mt_compiler_deinit(c);
    return NULL;
  }
  
  return c;
}

void mt_compiler_deinit(mt_compiler_t* c) {
  if (!c) return;
  
  for (size_t i = 0; i < l_size(c->files); i++) {
    mt_file_deinit(l_at(c->files, i));
  }
  l_deinit(c->files);
  
  for (size_t i = 0; i < l_size(c->t_unit); i++) {
    mt_translation_unit_deinit(l_at(c->t_unit, i));
  }
  l_deinit(c->t_unit);
  
  if (c->ast) {
    // TODO Deinit ast.
  }
  
  free(c);
}

bool mt_compiler_parse_exec(mt_compiler_t* c) {
  c->settings->stage = MT_STAGE_PARSE;
  
  // TODO Use thread pool to parse multiple source files simultaneously.
  for (size_t i = 0; i < l_size(c->files); ++i) {
    mt_translation_unit_t* t_unit = mt_translation_unit_init(l_at(c->files, i), c->settings);
    if (!t_unit) {
      return false;
    }
    
    bool parse_res = mt_translation_unit_parse_exec(t_unit);
    if (!parse_res) {
      return false;
    }
    
    // TODO Concatenate t_unit->ast with c->ast.
  }

  return true;
}

bool mt_compiler_semantic_analysis_exec(mt_compiler_t* c) {
  return false;
}

bool mt_compiler_irgen_exec(mt_compiler_t* c) {
  return false;
}
