#include <mutiny/mutiny.h>
#include <mutiny/translation_unit/parser/parser.h>
#include <mutiny/translation_unit/lexer/token.h>
#include <mutiny/translation_unit/parser/parser_util.h>
#include <mutiny/error/error_reporter.h>
#include <mutiny/ast/ast.h>
#include <mutiny/util/list.h>

#include <mutiny/translation_unit/parser/func_call.h>
#include <mutiny/translation_unit/parser/expression.h>

static mt_ast_node_t* mt_parse_func_call_param_list(mt_token_t** toks, mt_error_reporter_t* error_reporter);

mt_ast_node_t* mt_parse_func_call(mt_token_t** toks, mt_error_reporter_t* err) {
  mt_token_t* tok = *toks;
  
  mt_ast_node_t* func_call_nd = NULL, *param_list_nd = NULL;
  
  do {
    // Function name.
    if (!mt_token_match(err, tok, 1, TK_IDENTIFIER)) break;
    char* name = tok->str_val;
    tok = tok->next;
    
    // (
    if (!mt_token_match_punct(err, tok, 1, PCT_LPAREN)) break;
    tok = tok->next;

    // Function parameters.
    param_list_nd = mt_parse_func_call_param_list(&tok, err);

    // )
    if (!mt_token_match_punct(err, tok, 1, PCT_RPAREN)) break;
    tok = tok->next;

    func_call_nd = mt_ast_node_init(ND_FUNC_CALL);
    func_call_nd->str_val = name;
    if (param_list_nd) {
      l_push(func_call_nd->sub, param_list_nd);
    }
  } while (0);
  
  if (!func_call_nd && param_list_nd) {
    mt_ast_node_deinit(param_list_nd);
  }

  *toks = tok;
  return func_call_nd;
}

static mt_ast_node_t* mt_parse_func_call_param_list(mt_token_t** toks, mt_error_reporter_t* err) {
  mt_token_t* tok = *toks;
  
  if (MT_TOK_COMP_PUNCT(tok, PCT_RPAREN)) return NULL;
  
  mt_ast_node_t* param_list_nd = NULL;
  mt_ast_node_t* expr_nd;
  
  do {
    // Expression.
    expr_nd = mt_parse_expr(&tok, err);
    if (!expr_nd) break;
    
    if (!param_list_nd) {
      param_list_nd = mt_ast_node_init(ND_FUNC_CALL_PARAM_LIST);
    }
    
    l_push(param_list_nd->sub, expr_nd);
    
    // , or )
    mt_punctuator_t p = mt_token_match_punct(err, tok, 2, PCT_COMMA, PCT_RPAREN);
    if (!p || p == PCT_RPAREN) break;
    tok = tok->next;
  } while (1);
  
  *toks = tok;
  return param_list_nd;
}