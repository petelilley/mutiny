#include <mutiny/mutiny.h>
#include <mutiny/parser/parser.h>
#include <mutiny/parser/translation_unit.h>
#include <mutiny/parser/token.h>
#include <mutiny/parser/lexer.h>
#include <mutiny/parser/parser_util.h>
#include <mutiny/ast/ast.h>
#include <mutiny/settings.h>
#include <mutiny/error/error.h>
#include <mutiny/util/list.h>
#include <mutiny/util/filesystem.h>

mt_ast_t* parse(mt_settings_t* s) {
  mt_token_t* t = NULL;
  mt_ast_t* ast = NULL;
  
  s->stage = STAGE_PARSE;
  
  size_t src_num = l_size(s->src_dirs);
  if (!src_num) {
    fprintf(stderr, MT_ERROR "no input files\n");
    s->exit_code = EXIT_ERR_FILE;
    return NULL;
  }
  
  const char* path;
  mt_file_t* f = NULL;
  mt_ast_t* a = NULL;
  
  // TODO Use thread pool to parse multiple source files simultaneously.
  for (size_t i = 0; i < src_num; ++i) {
    path = l_at(s->src_dirs, i);
    
    if (is_directory(path)) {
      // TODO Something.
    }
    
    f = file_open(path);
    if (!f) {
      s->exit_code = EXIT_ERR_FILE;
      return NULL;
    }
    
    if (!END(path, ".mutiny")) {
      fprintf(stderr, "\033[1m%s:\033[0m " MT_WARNING "filename extension does not match '.mutiny' but will be treated as a mutiny source file.\n", path);
    }
    
    t = tokenize(f, s);
    if (!t) {
      // TODO Cleanup file.
      return NULL;
    }
    
    a = parse_tokens(t, s);
    if (!a) {
      // TODO Cleanup file.
      // TODO Cleanup tokens.
      return NULL;
    }
    
    // TODO Concatenate a to ast.
    
    // TODO Cleanup file.
    // TODO Cleanup tokens.
  }
  
  return ast;
}
