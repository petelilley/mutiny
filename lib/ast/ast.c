#include <mutiny/ast/ast.h>
#include <mutiny/mutiny.h>
#include <mutiny/util/list.h>

mt_operator_t mt_punct_to_operator(mt_punctuator_t p) {
  switch (p) {
    case '[': return OP_ARRAY;
    case '.': return OP_MEMBER;
    case '=': return OP_ASGN;
    case '+': return OP_ADD;
    case '-': return OP_MIN;
    case '*': return OP_MUL;
    case '/': return OP_DIV;
    case '%': return OP_MOD;
    case '&': return OP_BIT_AND;
    case '|': return OP_BIT_OR;
    case '^': return OP_BIT_XOR;
    case '~': return OP_BIT_NOT;
    case '>': return OP_CMP_GT;
    case '<': return OP_CMP_LT;
    case '!': return OP_LOG_NOT;
    case '@': return OP_REF;
    case PCT_BIT_LSH:      return OP_BIT_LSH;
    case PCT_BIT_RSH:      return OP_BIT_RSH;
    case PCT_ASGN_ADD:     return OP_ASGN_ADD;
    case PCT_ASGN_MIN:     return OP_ASGN_MIN;
    case PCT_ASGN_MUL:     return OP_ASGN_MUL;
    case PCT_ASGN_DIV:     return OP_ASGN_DIV;
    case PCT_ASGN_MOD:     return OP_ASGN_MOD;
    case PCT_ASGN_BIT_AND: return OP_ASGN_BIT_AND;
    case PCT_ASGN_BIT_OR:  return OP_ASGN_BIT_OR;
    case PCT_ASGN_BIT_XOR: return OP_ASGN_BIT_XOR;
    case PCT_ASGN_BIT_LSH: return OP_ASGN_BIT_LSH;
    case PCT_ASGN_BIT_RSH: return OP_ASGN_BIT_RSH;
    case PCT_CMP_EQ:       return OP_CMP_EQ;
    case PCT_CMP_NE:       return OP_CMP_NE;
    case PCT_CMP_LE:       return OP_CMP_LE;
    case PCT_CMP_GE:       return OP_CMP_GE;
    case PCT_LOG_OR:       return OP_LOG_OR;
    case PCT_LOG_AND:      return OP_LOG_AND;
    case PCT_INC:          return OP_POST_INC;
    case PCT_DEC:          return OP_POST_DEC;
    default:               return OP_UNKNOWN;
  }
}

mt_ast_node_t* mt_ast_node_init(mt_ast_type_t type) {
  mt_ast_node_t* ast = malloc(sizeof(mt_ast_node_t));
  memset(ast, 0, sizeof(mt_ast_node_t));
  ast->type = type;
  
  l_init(ast->sub, mt_ast_node_t*);
  
  return ast;
}

void mt_ast_node_deinit(mt_ast_node_t* ast) {
  if (!ast) return;
  
  for (size_t i = 0; i < l_size(ast->sub); i++) {
    mt_ast_node_deinit(l_at(ast->sub, i));
  }
  
  free(ast);
}

