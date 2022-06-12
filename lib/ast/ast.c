#include <mutiny/ast/ast.h>
#include <mutiny/mutiny.h>
#include <mutiny/util/list.h>

mt_ast_node_t* mt_ast_node_init(mt_ast_type_t type) {
  mt_ast_node_t* ast = malloc(sizeof(mt_ast_node_t));
  memset(ast, 0, sizeof(mt_ast_node_t));
  ast->type = type;
  
  l_init(ast->sub);
  
  return ast;
}

void mt_ast_node_deinit(mt_ast_node_t* ast) {
  if (!ast) return;
  
  if (ast->strval) {
    free(ast->strval);
  }
  
  for (size_t i = 0; i < l_size(ast->sub); i++) {
    mt_ast_node_deinit(l_at(ast->sub, i));
  }
  
  free(ast);
}

