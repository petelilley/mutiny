#include <mutiny/mutiny.h>
#include <mutiny/translation_unit/parser/parser.h>
#include <mutiny/translation_unit/lexer/token.h>
#include <mutiny/translation_unit/parser/parser_util.h>
#include <mutiny/error/error_reporter.h>
#include <mutiny/ast/ast.h>
#include <mutiny/util/list.h>

#include <mutiny/translation_unit/parser/func_decl.h>
#include <mutiny/translation_unit/parser/statement.h>

static mt_ast_node_t* mt_parse_func_decl_param_list(mt_token_t** toks, mt_error_reporter_t* error_reporter);

mt_ast_node_t* mt_parse_func_decl(mt_token_t** toks, mt_error_reporter_t* err) {
  mt_token_t* tok = *toks;
  
  mt_ast_node_t* param_list_nd;
  
  do {
    // func
    if (!mt_tok_kw_match(err, tok, 1, KW_FUNC)) break;
    tok = tok->next;
    
    // Function name.
    if (!mt_tok_match(err, tok, 1, TK_IDENTIFIER)) break;
    const char* name = tok->str_val;
    tok = tok->next;
    
    // :
    if (!mt_tok_punct_match(err, tok, 1, PCT_COLON)) break;
    tok = tok->next;
    
    // (
    if (!mt_tok_punct_match(err, tok, 1, PCT_LPAREN)) break;
    tok = tok->next;
    
    // Function parameters.
    param_list_nd = mt_parse_func_decl_param_list(&tok, err);
    
    // )
    if (!mt_tok_punct_match(err, tok, 1, PCT_RPAREN)) break;
    tok = tok->next;
    
    const char* ret_type = "void";
    
    // ; or -> or {
    mt_punctuator_t p = mt_tok_punct_match(err, tok, 3, PCT_SEMICOLON, PCT_ARROW, PCT_LBRACKET);

END_DECL:
    if (!p) break;
    
    // ;
    if (p == PCT_SEMICOLON) {
      // No definition.
      tok = tok->next;
      break;
    }
    // ->
    else if (p == PCT_ARROW) {
      tok = tok->next;
      // Return type.
      if (!mt_tok_match(err, tok, 1, TK_IDENTIFIER)) break;
      ret_type = tok->str_val;
      tok = tok->next;
    
      // ; or {
      p = mt_tok_punct_match(err, tok, 2, PCT_SEMICOLON, PCT_LBRACKET);
      
      goto END_DECL;
    }
    // {
    else if (p == PCT_LBRACKET) {
      // Definition.
      mt_ast_node_t* body = mt_parse_compound_stmt(&tok, err);
      if (body) {
          // TODO: Add body to func ast node.
      }
    }

#if 1
    printf("function\n\tname: %s\n\treturns: %s\n", name, ret_type);
    
    if (param_list_nd) {
      for (size_t i = 0; i < l_size(param_list_nd->sub); i++) {
        if (l_size(l_at(param_list_nd->sub, i)->sub) == 1) {
          printf("\ttype: %s\n", l_at(l_at(param_list_nd->sub, i)->sub, 0)->str_val);
        }
        else {
          printf("\tname: %s, type: %s\n", l_at(l_at(param_list_nd->sub, i)->sub, 0)->str_val, l_at(l_at(param_list_nd->sub, i)->sub, 1)->str_val);
        }
      }
    }
#endif

  } while (0);

  *toks = tok;
  
  return NULL;
}

static mt_ast_node_t* mt_parse_func_decl_param_list(mt_token_t** toks, mt_error_reporter_t* err) {
  mt_token_t* tok = *toks;
  
  if (mt_tok_punct_comp(tok, PCT_RPAREN)) return NULL;
  
  mt_ast_node_t* param_list_nd = NULL;
  mt_ast_node_t* param_nd, *name_nd, *type_nd;
  
  char* name, *type;
  do {
    // Param Name
    if (!mt_tok_match(err, tok, 1, TK_IDENTIFIER)) break;
    name = tok->str_val;
    tok = tok->next;
    
    // :
    if (!mt_tok_punct_comp(tok, PCT_COLON)) {
      // No colon, so unnamed parameter, therefore the first identifier is the type.
      type = name;
      name = NULL;
    }
    else {
      // Param Type.
      tok = tok->next;
      // TODO: Dedicated type parser.
      if (!mt_tok_match(err, tok, 1, TK_IDENTIFIER)) break;
      type = tok->str_val;
      tok = tok->next;
    }
    
    if (!param_list_nd) {
      param_list_nd = mt_ast_node_init(ND_FUNC_DECL_PARAM_LIST);
    }
    
    if (name) {
      name_nd = mt_ast_node_init(ND_VAR_NAME);
      name_nd->str_val = name;
    }
    
    type_nd = mt_ast_node_init(ND_VAR_TYPE);
    type_nd->str_val = type;
    
    param_nd = mt_ast_node_init(ND_FUNC_DECL_PARAM);
    if (name) {
      l_push(param_nd->sub, name_nd);
    }
    l_push(param_nd->sub, type_nd);
    l_push(param_list_nd->sub, param_nd);
    
    // , or )
    mt_punctuator_t p = mt_tok_punct_match(err, tok, 2, PCT_COMMA, PCT_RPAREN);
    if (!p || p == PCT_RPAREN) break;
    tok = tok->next;
  } while (1);
  
  *toks = tok;
  
  return param_list_nd;
}