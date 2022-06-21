#include <mutiny/mutiny.h>
#include <mutiny/translation_unit/parser/parser.h>
#include <mutiny/translation_unit/lexer/token.h>
#include <mutiny/translation_unit/parser/parser_util.h>
#include <mutiny/error/syntax_error.h>
#include <mutiny/error/error_reporter.h>
#include <mutiny/ast/ast.h>
#include <mutiny/util/list.h>

#include <mutiny/translation_unit/parser/expression.h>
#include <mutiny/translation_unit/parser/func_call.h>

/**
 * @brief A fragment of an expression.
 */
typedef struct _mt_expr_fragment {
  enum {
    EXPR_FRAG_UNIT,
    EXPR_FRAG_OP,
  } type;
  
  union {
    mt_ast_node_t* op_nd;
    mt_ast_node_t* unit_nd;
  } data;
} mt_expr_fragment_t;

static mt_ast_node_t* mt_parse_expr_unit(mt_token_t** toks, mt_error_reporter_t* err) {
  mt_token_t* tok = *toks;

  mt_ast_node_t* unit_nd = NULL;

  do {
    // TODO: Check for prefix operators (++, --, @, *, !, +, -, etc).
    
    mt_token_kind_t k = mt_token_match(err, tok, 4, TK_IDENTIFIER, TK_NUMBER, TK_STRING, TK_CHAR);
    if (!k) break;
    if (k == TK_IDENTIFIER) {
      if (MT_TOK_COMP_PUNCT(tok->next, '(')) {
        unit_nd = mt_parse_func_call(&tok, err);
        if (!unit_nd) break;
      }
      else {
        unit_nd = mt_ast_node_init(ND_ID);
        unit_nd->str_val = tok->str_val;
        tok = tok->next;
        if (!unit_nd) break;
      }
    }
    else if (k == TK_NUMBER) {
      if (tok->i_val != MT_UNDEFINED) {
        unit_nd = mt_ast_node_init(ND_INT_LITERAL);
        unit_nd->i_val = tok->i_val;
      }
      else if (tok->f_val != MT_UNDEFINED) {
        unit_nd = mt_ast_node_init(ND_FLOAT_LITERAL);
        unit_nd->f_val = tok->f_val;
      }
      tok = tok->next;
    }
    else if (k == TK_STRING) {
      unit_nd = mt_ast_node_init(ND_STRING_LITERAL);
      unit_nd->str_val = tok->str_val;
      tok = tok->next;
    }
    else if (k == TK_CHAR) {
      unit_nd = mt_ast_node_init(ND_CHAR_LITERAL);
      unit_nd->c_val = tok->c_val;
      tok = tok->next;
    }
    
    // TODO: Check for suffix operators (++, --, etc).
  } while (0);

  *toks = tok;
  return unit_nd;
}

static mt_ast_node_t* mt_parse_operator(mt_token_t** toks, mt_error_reporter_t* err) {
  mt_token_t* tok = *toks;

  mt_ast_node_t* op_nd = NULL;

  do {
    if (!mt_token_match(err, tok, 1, TK_PUNCTUATOR)) break;
    mt_punctuator_t p = tok->punct_val;
    
    switch (p) {
      // --- Arithmetic operators ---
      case '+':
      case '-':
      case '*':
      case '/':
      case '%':
      case '&':
      case '|':
      case '^':
      case PCT_BIT_LSH: // <<
      case PCT_BIT_RSH: // >>
        op_nd = mt_ast_node_init(ND_OP_ARITH);
        break;
      // --- Assignment operators ---
      case '=':
      case PCT_ASGN_ADD:     // +=
      case PCT_ASGN_MIN:     // -=
      case PCT_ASGN_MUL:     // *=
      case PCT_ASGN_DIV:     // /=
      case PCT_ASGN_MOD:     // %=
      case PCT_ASGN_BIT_AND: // &=
      case PCT_ASGN_BIT_OR:  // |=
      case PCT_ASGN_BIT_XOR: // ^=
      case PCT_ASGN_BIT_LSH: // <<=
      case PCT_ASGN_BIT_RSH: // >>=
        op_nd = mt_ast_node_init(ND_OP_ASSIGN);
        break;
      // --- Comparison operators ---
      case PCT_CMP_EQ:  // ==
      case PCT_CMP_NE: // !=
      case '<':
      case '>':
      case PCT_CMP_LE:    // <=
      case PCT_CMP_GE: // >=
        op_nd = mt_ast_node_init(ND_OP_CMP);
        break;
      // --- Logical operators ---
      case PCT_LOG_AND: // &&
      case PCT_LOG_OR:  // ||
        op_nd = mt_ast_node_init(ND_OP_LOG);
        break;
      // --- Not an operator ---
      default:
        break;
    }
  } while (0);
  
  if (op_nd) {
    op_nd->i_val = mt_punct_to_operator(tok->punct_val);
    tok = tok->next;
  }
  
  *toks = tok;
  return op_nd;
}

mt_ast_node_t* mt_parse_expr(mt_token_t** toks, mt_error_reporter_t* err) {
  mt_token_t* tok = *toks;
  
  mt_ast_node_t* expr_nd = NULL;

  list_t(mt_expr_fragment_t) expr_frags;
  l_init(expr_frags, mt_expr_fragment_t);
  
  mt_ast_node_t* tmp_nd = NULL;
  mt_expr_fragment_t f;
  
  bool expect_unit = true;
  do {
    if (expect_unit) {
      tmp_nd = mt_parse_expr_unit(&tok, err);
      if (!tmp_nd) break;
      
      f = (mt_expr_fragment_t){ .type = EXPR_FRAG_UNIT, .data.unit_nd = tmp_nd };
      expect_unit = false;
    }
    else {
      tmp_nd = mt_parse_operator(&tok, err);
      if (!tmp_nd) break;
      
      f = (mt_expr_fragment_t){ .type = EXPR_FRAG_OP, .data.unit_nd = tmp_nd };
      expect_unit = true;
    }
    
    l_push(expr_frags, f);
  } while (1);
  
  for (size_t i = 0; i < l_size(expr_frags); i++) {
    mt_expr_fragment_t* x = &l_at(expr_frags, i);
    if (x->type == EXPR_FRAG_UNIT) {
      printf("unit: %d\n", x->data.unit_nd->type);
    }
    else {
      printf("op: %lld\n", x->data.op_nd->i_val);
    }
  }

  // TODO: Generate AST node representing the expression from the fragments.

  *toks = tok;
  return NULL;
}