#include <mutiny/mutiny.h>
#include <mutiny/translation_unit/parser/parser.h>
#include <mutiny/translation_unit/lexer/token.h>
#include <mutiny/translation_unit/parser/parser_util.h>
#include <mutiny/error/error_reporter.h>
#include <mutiny/ast/ast.h>
#include <mutiny/util/list.h>

#include <mutiny/translation_unit/parser/func_decl.h>
#include <mutiny/translation_unit/parser/statement.h>

static mt_ast_node_t* mt_parse_func_decl_param_list(mt_token_t** toks, mt_error_reporter_t* err_reporter);

mt_ast_node_t* mt_parse_func_decl(mt_token_t** toks, mt_error_reporter_t* err) {
  mt_token_t* tok = *toks;
  
  mt_ast_node_t* param_list_nd;
  
  do {
    if (!mt_tok_kw_match(tok, KW_FUNC, err)) break;
    tok = tok->next;
    
    if (!mt_tok_match(tok, TK_IDENTIFIER, err)) break;
    const char* name = tok->strval;
    tok = tok->next;
    
    if (!mt_tok_punct_match(tok, ":", err)) break;
    tok = tok->next;
    
    if (!mt_tok_punct_match(tok, "(", err)) break;
    tok = tok->next;
    
    param_list_nd = mt_parse_func_decl_param_list(&tok, err);
    
    if (!mt_tok_punct_match(tok, ")", err)) break;
    tok = tok->next;
    
    // Return type.
    const char* ret_type = "void";
    
    if (mt_tok_punct_comp(tok, "->")) {
      tok = tok->next;
      
      if (!mt_tok_match(tok, TK_IDENTIFIER, err)) break;
      ret_type = tok->strval;
      tok = tok->next;
    }
    
    printf("function\n\tname: %s\n\treturns: %s\n", name, ret_type);
    
    if (param_list_nd) {
      for (size_t i = 0; i < l_size(param_list_nd->sub); i++) {
        printf("\tparam name: %s, type: %s\n", l_at(l_at(param_list_nd->sub, i)->sub, 0)->strval, l_at(l_at(param_list_nd->sub, i)->sub, 1)->strval);
      }
    }

    if (mt_tok_punct_comp(tok, ";")) {
        tok = tok->next;
        break;
    }

    if (!mt_tok_punct_match(tok, "{", err)) break;
    
    mt_ast_node_t* body = mt_parse_compound_stmt(&tok, err);
    if (body) {
        // TODO: Add body to func ast node.
    }
  } while (0);

  *toks = tok;
  
  return NULL;
}

static mt_ast_node_t* mt_parse_func_decl_param_list(mt_token_t** toks, mt_error_reporter_t* err) {
  mt_token_t* tok = *toks;
  
  if (mt_tok_punct_comp(tok, ")")) return NULL;
  
  mt_ast_node_t* param_list_nd = NULL;
  mt_ast_node_t* param_nd, *name_nd, *type_nd;
  
  do {
    if (!mt_tok_match(tok, TK_IDENTIFIER, err)) break;
    char* name = tok->strval;
    tok = tok->next;
    
    if (!mt_tok_punct_match(tok, ":", err)) break;
    tok = tok->next;
    
    // TODO: Dedicated type parser.
    
    if (!mt_tok_match(tok, TK_IDENTIFIER, err)) break;
    char* type = tok->strval;
    tok = tok->next;
    
    if (!param_list_nd) {
      param_list_nd = mt_ast_node_init(ND_FUNC_DECL_PARAM_LIST);
    }
    
    name_nd = mt_ast_node_init(ND_VAR_NAME);
    name_nd->strval = name;
    
    type_nd = mt_ast_node_init(ND_VAR_TYPE);
    type_nd->strval = type;
    
    param_nd = mt_ast_node_init(ND_FUNC_DECL_PARAM);
    l_push(param_nd->sub, name_nd);
    l_push(param_nd->sub, type_nd);
    l_push(param_list_nd->sub, param_nd);
    
    if (mt_tok_punct_comp(tok, ")")) break;
    
    if (!mt_tok_punct_match(tok, ",", err)) break;
    tok = tok->next;
  } while (1);
  
  *toks = tok;
  
  return param_list_nd;
}