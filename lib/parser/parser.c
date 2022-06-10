#include <mutiny/mutiny.h>
#include <mutiny/parser/parser.h>
#include <mutiny/parser/token.h>
#include <mutiny/ast/ast.h>
#include <mutiny/settings.h>
#include <mutiny/error/error.h>
#include <mutiny/util/list.h>

static mt_ast_t* parse_global_expr(mt_token_t* token, mt_settings_t* settings);

mt_ast_t* parse_tokens(mt_token_t* t, mt_settings_t* s) {
  mt_ast_t* ast = NULL;
  
  return ast;
}

static mt_ast_t* parse_global_expr(mt_token_t* t, mt_settings_t* s) {
  // TODO import, enum, struct, union, function, type alias, global variable
}
