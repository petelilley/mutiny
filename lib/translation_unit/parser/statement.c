#include <mutiny/mutiny.h>
#include <mutiny/translation_unit/parser/parser.h>
#include <mutiny/translation_unit/lexer/token.h>
#include <mutiny/translation_unit/parser/parser_util.h>
#include <mutiny/error/error_reporter.h>
#include <mutiny/ast/ast.h>
#include <mutiny/util/list.h>

#include <mutiny/translation_unit/parser/statement.h>
#include <mutiny/translation_unit/parser/expression.h>
#include <mutiny/translation_unit/parser/var_decl.h>

static mt_ast_node_t* mt_parse_stmt_list(mt_token_t** toks, mt_error_reporter_t* err_reporter);
static mt_ast_node_t* mt_parse_label(mt_token_t** toks, mt_error_reporter_t* err_reporter);
static mt_ast_node_t* mt_parse_if_stmt(mt_token_t** toks, mt_error_reporter_t* err_reporter);
static mt_ast_node_t* mt_parse_while_stmt(mt_token_t** toks, mt_error_reporter_t* err_reporter);
static mt_ast_node_t* mt_parse_for_stmt(mt_token_t** toks, mt_error_reporter_t* err_reporter);
static mt_ast_node_t* mt_parse_switch_stmt(mt_token_t** toks, mt_error_reporter_t* err_reporter);
static mt_ast_node_t* mt_parse_goto_stmt(mt_token_t** toks, mt_error_reporter_t* err_reporter);
static mt_ast_node_t* mt_parse_return_stmt(mt_token_t** toks, mt_error_reporter_t* err_reporter);
static mt_ast_node_t* mt_parse_break_stmt(mt_token_t** toks, mt_error_reporter_t* err_reporter);
static mt_ast_node_t* mt_parse_continue_stmt(mt_token_t** toks, mt_error_reporter_t* err_reporter);

mt_ast_node_t* mt_parse_stmt(mt_token_t** toks, mt_error_reporter_t* err) {
  mt_token_t* tok = *toks;
  
  mt_token_kind_t kind = mt_token_match(err, tok, 7, TK_IDENTIFIER, TK_KEYWORD, TK_PUNCTUATOR, TK_NUMBER, TK_STRING, TK_CHAR);
  if (!kind) return NULL;

  mt_ast_node_t* stmt = NULL;
  
  do {
    if (kind == TK_IDENTIFIER) {
        // :
        if (MT_TOK_COMP_PUNCT(tok->next, ':')) {
          // Label
          stmt = mt_parse_label(&tok, err);
          puts("label");
        }
        else {
          // Expression.
          stmt = mt_parse_expr(&tok, err);
          bool sc = mt_token_match_punct(err, tok, 1, ';');
          tok = tok->next;
          if (!sc) break;
          puts("expression");
        }
    }
    else if (kind == TK_KEYWORD) {
      mt_keyword_t kw = mt_token_match_kw(err, tok, 9, KW_VAR, KW_IF, KW_WHILE, KW_FOR, KW_SWITCH, KW_GOTO, KW_RETURN, KW_BREAK, KW_CONTINUE);
      if (!kw) break;
      
      // var
      if (kw == KW_VAR) {
        stmt = mt_parse_var_decl(&tok, err);
        puts("var");
      }
      // if
      else if (kw == KW_IF) {
        stmt = mt_parse_if_stmt(&tok, err);
        puts("if");
      }
      // while
      else if (kw == KW_WHILE) {
        stmt = mt_parse_while_stmt(&tok, err);
        puts("while");
      }
      // for
      else if (kw == KW_FOR) {
        stmt = mt_parse_for_stmt(&tok, err);
        puts("for");
      }
      // switch
      else if (kw == KW_SWITCH) {
        stmt = mt_parse_switch_stmt(&tok, err);
        puts("switch");
      }
      // goto
      else if (kw == KW_GOTO) {
        stmt = mt_parse_goto_stmt(&tok, err);
        puts("goto");
      }
      // return
      else if (kw == KW_RETURN) {
        stmt = mt_parse_return_stmt(&tok, err);
        puts("return");
      }
      // break
      else if (kw == KW_BREAK) {
        stmt = mt_parse_break_stmt(&tok, err);
        puts("break");
      }
      // continue
      else if (kw == KW_CONTINUE) {
        stmt = mt_parse_continue_stmt(&tok, err);
        puts("continue");
      }
    }
    else {
      if (MT_TOK_COMP_PUNCT(tok, ';')) {
        stmt = NULL;
        tok = tok->next;
        break;
      }
      // Expression.
      stmt = mt_parse_expr(&tok, err);
      if (!stmt) break;
      bool sc = mt_token_match_punct(err, tok, 1, ';');
      tok = tok->next;
      if (!sc) break;
      puts("expression 2");
    }
  } while (0);

  *toks = tok;
  return stmt;
}

mt_ast_node_t* mt_parse_compound_stmt(mt_token_t** toks, mt_error_reporter_t* err) {
  mt_token_t* tok = *toks;
  
  mt_ast_node_t* stmt_list_nd = NULL;
  
  do {
    // {
    if (!mt_token_match_punct(err, tok, 1, '{')) break;
    tok = tok->next;
    
    // Statements.
    stmt_list_nd = mt_parse_stmt_list(&tok, err);
    if (!stmt_list_nd) break;
    
    // }
    if (!mt_token_match_punct(err, tok, 1, '}')) break;
    tok = tok->next;
  } while (0);
  
  *toks = tok;
  return stmt_list_nd;
}

static mt_ast_node_t* mt_parse_stmt_list(mt_token_t** toks, mt_error_reporter_t* err) {
  mt_token_t* tok = *toks;
  
  // }
  if (MT_TOK_COMP_PUNCT(tok, '}')) return NULL;
  
  mt_ast_node_t* comp_stmt_nd = NULL;
  mt_ast_node_t* stmt_nd;
  
  do {
    // Statement.
    stmt_nd = mt_parse_stmt(&tok, err);
    if (!stmt_nd) break;
    
    if (!comp_stmt_nd) {
      comp_stmt_nd = mt_ast_node_init(ND_STMT_COMPOUND);
    }
    
    l_push(comp_stmt_nd->sub, stmt_nd);
    
    // }
    if (MT_TOK_COMP_PUNCT(tok, '}')) break;
  } while (1);
  
  *toks = tok;
  return comp_stmt_nd;
}

static mt_ast_node_t* mt_parse_label(mt_token_t** toks, mt_error_reporter_t* err) {
  mt_token_t* tok = *toks;
  
  mt_ast_node_t* label_nd = NULL;

  char* name;

  do {
    // Name.
    if (!mt_token_match(err, tok, 1, TK_IDENTIFIER)) break;
    name = tok->str_val;
    tok = tok->next;
    
    // :
    if (!mt_token_match_punct(err, tok, 1, ':')) break;
    tok = tok->next;
    
    label_nd = mt_ast_node_init(ND_LABEL);
    label_nd->str_val = name;
  } while (0);

  *toks = tok;
  return label_nd;
}

static mt_ast_node_t* mt_parse_if_stmt(mt_token_t** toks, mt_error_reporter_t* err) {
  mt_token_t* tok =   *toks;
  
  mt_ast_node_t* if_nd = NULL, *expr_nd = NULL, *stmt_nd = NULL;

  do {
    // if
    if (!mt_token_match_kw(err, tok, 1, KW_IF)) break;
    tok = tok->next;
    
    // (
    if (!mt_token_match_punct(err, tok, 1, '(')) break;
    tok = tok->next;
    
    // Expression.
    expr_nd = mt_parse_expr(&tok, err);
    if (!expr_nd) break;
    
    // )
    if (!mt_token_match_punct(err, tok, 1, ')')) break;
    tok = tok->next;
    
    stmt_nd = mt_parse_stmt(&tok, err);
    if (!stmt_nd) break;
    
    if_nd = mt_ast_node_init(ND_STMT_IF);
    l_push(if_nd->sub, expr_nd);
    l_push(if_nd->sub, stmt_nd);
  } while (0);
  
  if (!if_nd) {
    if (expr_nd) {
      mt_ast_node_deinit(expr_nd);
    }
  }

  *toks = tok;
  return NULL;
}

static mt_ast_node_t* mt_parse_while_stmt(mt_token_t** toks, mt_error_reporter_t* err) {
  mt_token_t* tok =   *toks;
  
  mt_ast_node_t* while_nd = NULL, *expr_nd = NULL, *stmt_nd = NULL;

  do {
    // while
    if (!mt_token_match_kw(err, tok, 1, KW_WHILE)) break;
    tok = tok->next;
    
    // (
    if (!mt_token_match_punct(err, tok, 1, '(')) break;
    tok = tok->next;
    
    // Expression.
    expr_nd = mt_parse_expr(&tok, err);
    if (!expr_nd) break;
    
    // )
    if (!mt_token_match_punct(err, tok, 1, ')')) break;
    tok = tok->next;
    
    stmt_nd = mt_parse_stmt(&tok, err);
    if (!stmt_nd) break;
    
    while_nd = mt_ast_node_init(ND_STMT_WHILE);
    l_push(while_nd->sub, expr_nd);
    l_push(while_nd->sub, stmt_nd);
  } while (0);
  
  if (!while_nd) {
    if (expr_nd) {
      mt_ast_node_deinit(expr_nd);
    }
  }

  *toks = tok;
  return NULL;
}

static mt_ast_node_t* mt_parse_for_stmt(mt_token_t** toks, mt_error_reporter_t* err) {
  mt_token_t* tok =   *toks;

  // D:

  *toks = tok;
  return NULL;
}

static mt_ast_node_t* mt_parse_switch_stmt(mt_token_t** toks, mt_error_reporter_t* err) {
  mt_token_t* tok =   *toks;

  // D:

  *toks = tok;
  return NULL;
}

static mt_ast_node_t* mt_parse_goto_stmt(mt_token_t** toks, mt_error_reporter_t* err) {
  mt_token_t* tok =   *toks;

  mt_ast_node_t* goto_nd = NULL;

  char* name;

  do {
    if (!mt_token_match_kw(err, tok, 1, KW_GOTO)) break;
    tok = tok->next;

    if (!mt_token_match(err, tok, 1, TK_IDENTIFIER)) break;
    name = tok->str_val;
    tok = tok->next;

    // ;
    if (!mt_token_match_punct(err, tok, 1, ';')) break;
    tok = tok->next;

    goto_nd = mt_ast_node_init(ND_STMT_GOTO);
    goto_nd->str_val = name;
  } while (0);

  *toks = tok;
  return goto_nd;
}

static mt_ast_node_t* mt_parse_return_stmt(mt_token_t** toks, mt_error_reporter_t* err) {
  mt_token_t* tok =   *toks;
  
  mt_ast_node_t* return_nd = NULL, *expr_nd = NULL;

  do {
    // return
    if (!mt_token_match_kw(err, tok, 1, KW_RETURN)) break;
    tok = tok->next;

    // ;
    if (MT_TOK_COMP_PUNCT(tok, ';')) {
      tok = tok->next;
    }
    // Expression.
    else {
      expr_nd = mt_parse_expr(&tok, err);
      if (!expr_nd) break;
      // ;
      if (!mt_token_match_punct(err, tok, 1, ';')) break;
      tok = tok->next;
    }
    
    return_nd = mt_ast_node_init(ND_STMT_RETURN);
    if (expr_nd) {
      l_push(return_nd->sub, expr_nd);
    }
  } while (0);

  *toks = tok;
  return return_nd;
}

static mt_ast_node_t* mt_parse_break_stmt(mt_token_t** toks, mt_error_reporter_t* err) {
  mt_token_t* tok =   *toks;
  mt_ast_node_t* break_nd = NULL;

  do {
    // break
    if (!mt_token_match_kw(err, tok, 1, KW_BREAK)) break;
    tok = tok->next;

    // ;
    if (!mt_token_match_punct(err, tok, 1, ';')) break;
    tok = tok->next;
    
    break_nd = mt_ast_node_init(ND_STMT_BREAK);
  } while (0);

  *toks = tok;
  return break_nd;
}

static mt_ast_node_t* mt_parse_continue_stmt(mt_token_t** toks, mt_error_reporter_t* err) {
  mt_token_t* tok =   *toks;
  mt_ast_node_t* continue_nd = NULL;

  do {
    // continue
    if (!mt_token_match_kw(err, tok, 1, KW_CONTINUE)) break;
    tok = tok->next;

    // ;
    if (!mt_token_match_punct(err, tok, 1, ';')) break;
    tok = tok->next;
    
    continue_nd = mt_ast_node_init(ND_STMT_CONTINUE);
  } while (0);

  *toks = tok;
  return continue_nd;
}