#include <mutiny/mutiny.h>
#include <mutiny/translation_unit/parser/parser.h>
#include <mutiny/translation_unit/lexer/token.h>
#include <mutiny/translation_unit/translation_unit.h>
#include <mutiny/ast/ast.h>
#include <mutiny/settings.h>
#include <mutiny/util/list.h>
#include <mutiny/translation_unit/parser/global_decl.h>

/*
enum {
  SCS_STATIC = 1 << 0,
};
typedef unsigned mt_storage_class_spec_t;

enum {
  TQ_CONST    = 1 << 0,
  TQ_VOLATILE = 1 << 1,
};
typedef unsigned mt_type_qual_t;
*/

bool mt_translation_unit_parse_tokens(mt_translation_unit_t* t_unit) {
  t_unit->ast = mt_ast_node_init(ND_GLOBAL_SCOPE);
  
  mt_token_t* tok = t_unit->tokens;
  mt_ast_node_t* node;
  while (tok->kind != TK_EOF) {
    node = mt_parse_global_decl(&tok);
    if (!node) break;
    l_push(t_unit->ast->sub, node);
  }
  
  if (!l_size(t_unit->ast->sub)) {
    // TODO: Error: translation unit required to contain at least one declaration.
  }
  
  return true;
}