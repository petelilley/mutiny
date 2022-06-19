#include <mutiny/mutiny.h>
#include <mutiny/translation_unit/parser/parser.h>
#include <mutiny/translation_unit/parser/parser_util.h>
#include <mutiny/translation_unit/lexer/token.h>
#include <mutiny/error/error_reporter.h>
#include <mutiny/ast/ast.h>
#include <mutiny/util/list.h>

#include <mutiny/translation_unit/parser/global_decl.h>
#include <mutiny/translation_unit/parser/enum_decl.h>
#include <mutiny/translation_unit/parser/struct_decl.h>
#include <mutiny/translation_unit/parser/func_decl.h>
#include <mutiny/translation_unit/parser/var_decl.h>

mt_ast_node_t* mt_parse_global_decl(mt_token_t** toks, mt_error_reporter_t* err) {
  mt_keyword_t k = mt_token_match_kw(err, *toks, 4, KW_ENUM, KW_STRUCT, KW_FUNC, KW_VAR);
  
  mt_ast_node_t* nd = NULL;
  
  switch (k) {
    case KW_ENUM:
      nd = mt_parse_enum_decl(toks, err);
      break;
    case KW_STRUCT:
      nd = mt_parse_struct_decl(toks, err);
      break;
    case KW_FUNC:
      nd = mt_parse_func_decl(toks, err);
      break;
    case KW_VAR:
      nd = mt_parse_var_decl(toks, err);
      break;
    default:
      break;
  }
  
  return nd;
}