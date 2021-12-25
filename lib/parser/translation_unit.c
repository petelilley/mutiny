#include <mutiny/mutiny.h>
#include <mutiny/parser/translation_unit.h>
#include <mutiny/parser/parser.h>
#include <mutiny/parser/lexer.h>
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
  
  // TODO Use thread pool to parse multiple source files simultaneously.
  for (size_t i = 0; i < src_num; ++i) {
    path = l_at(s->src_dirs, i);
    
    if (is_directory(path)) {
      // TODO Something.
    }
    
    if (!(f = file_open(path))) {
      s->exit_code = EXIT_ERR_FILE;
      return NULL;
    }
    
    if (!(t = tokenize(f, s))) {
      // TODO Cleanup file.
      return NULL;
    }
    
    if (!(ast = parse_tokens(t, s))) {
      // TODO Cleanup file.
      // TODO Cleanup tokens.
      return NULL;
    }
    
    // TODO Cleanup file.
    // TODO Cleanup tokens.
  }
  
  return ast;
}
