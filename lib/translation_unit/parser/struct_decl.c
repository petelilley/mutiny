#include <mutiny/mutiny.h>
#include <mutiny/translation_unit/parser/parser.h>
#include <mutiny/translation_unit/lexer/token.h>
#include <mutiny/translation_unit/parser/parser_util.h>
#include <mutiny/error/error_reporter.h>
#include <mutiny/ast/ast.h>
#include <mutiny/util/list.h>

#include <mutiny/translation_unit/parser/struct_decl.h>

mt_ast_node_t* mt_parse_struct_decl(mt_token_t** toks, mt_error_reporter_t* err) {
  mt_token_t* tok = *toks;

  *toks = tok;
  return NULL;
}