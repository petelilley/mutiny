#include <mutiny/mutiny.h>
#include <mutiny/translation_unit/parser/parser.h>
#include <mutiny/translation_unit/lexer/token.h>
#include <mutiny/translation_unit/parser/parser_util.h>
#include <mutiny/error/error_reporter.h>
#include <mutiny/ast/ast.h>
#include <mutiny/util/list.h>

#include <mutiny/translation_unit/parser/statement.h>

static mt_ast_node_t* mt_parse_stmt_list(mt_token_t** toks, mt_error_reporter_t* err_reporter);

mt_ast_node_t* mt_parse_stmt(mt_token_t** toks, mt_error_reporter_t* err) {
  mt_token_t* tok = *toks;

  *toks = tok;
  return NULL;
}

mt_ast_node_t* mt_parse_compound_stmt(mt_token_t** toks, mt_error_reporter_t* err) {
  mt_token_t* tok = *toks;
  
  mt_ast_node_t* stmt_list_nd = NULL;
  
  do {
    if (!mt_tok_punct_match(err, tok, 1, PCT_LBRACKET)) break;
    tok = tok->next;
    
    stmt_list_nd = mt_parse_stmt_list(&tok, err);
    
    if (!mt_tok_punct_match(err, tok, 1, PCT_RBRACKET)) break;
    tok = tok->next;
  } while (0);
  
  *toks = tok;
  return stmt_list_nd;
}

static mt_ast_node_t* mt_parse_stmt_list(mt_token_t** toks, mt_error_reporter_t* err) {
  mt_token_t* tok = *toks;
  
  mt_ast_node_t* comp_stmt_nd = NULL;
  mt_ast_node_t* stmt_nd;
  
  do {
    stmt_nd = mt_parse_stmt(&tok, err);
    
    if (!comp_stmt_nd) {
      comp_stmt_nd = mt_ast_node_init(ND_STMT_COMPOUND);
    }
    
    if (stmt_nd) {
      l_push(comp_stmt_nd->sub, stmt_nd);
    }
    
    if (mt_tok_punct_comp(tok, PCT_RBRACKET)) break;
  } while (1);
  
  *toks = tok;
  return comp_stmt_nd;
}