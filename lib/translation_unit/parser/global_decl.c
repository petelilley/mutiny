#include <mutiny/mutiny.h>
#include <mutiny/translation_unit/parser/parser.h>
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
  if ((*toks)->kind == TK_KEYWORD) {
    switch ((mt_keyword_t)(*toks)->ival) {
      case KW_ENUM:
        // return mt_parse_enum_decl(toks);
        return NULL;
      case KW_STRUCT:
        // return mt_parse_struct_decl(toks);
        return NULL;
      case KW_FUNC:
        return mt_parse_func_decl(toks, err);
      case KW_VAR:
        // return mt_parse_var_decl(toks);
        return NULL;
      default:
        // TODO: Error.
        break;
    }
  }
  else {
  }
  
  return NULL;
}