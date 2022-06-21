#include <mutiny/ast/ast.h>
#include <mutiny/mutiny.h>
#include <mutiny/util/list.h>
#include <mutiny/util/log.h>

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

size_t mt_get_operator_precedence(mt_operator_t op) {
  switch (op) {
    case OP_PRE_INC:
    case OP_PRE_DEC:
    case OP_MEMBER:
    case OP_ARRAY:
      return 0;
    case OP_BIT_NOT:
    case OP_LOG_NOT:
    case OP_REF:
    case OP_DEREF:
    case OP_POST_INC:
    case OP_POST_DEC:
    case OP_POS:
    case OP_NEG:
      return 1;
    case OP_MUL:
    case OP_DIV:
    case OP_MOD:
      return 2;
    case OP_ADD:
    case OP_MIN:
      return 3;
    case OP_BIT_LSH:
    case OP_BIT_RSH:
      return 4;
    case OP_CMP_GT:
    case OP_CMP_GE:
    case OP_CMP_LT:
    case OP_CMP_LE:
      return 5;
    case OP_CMP_EQ:
    case OP_CMP_NE:
      return 6;
    case OP_BIT_AND:
      return 7;
    case OP_BIT_XOR:
      return 8;
    case OP_BIT_OR:
      return 9;
    case OP_LOG_AND:
      return 10;
    case OP_LOG_OR:
      return 11;
    case OP_ASGN:
    case OP_ASGN_ADD:
    case OP_ASGN_MIN:
    case OP_ASGN_MUL:
    case OP_ASGN_DIV:
    case OP_ASGN_MOD:
    case OP_ASGN_BIT_AND:
    case OP_ASGN_BIT_OR:
    case OP_ASGN_BIT_XOR:
    case OP_ASGN_BIT_LSH:
    case OP_ASGN_BIT_RSH:
      return 12;
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

void mt_ast_node_dump(mt_log_t* l, mt_ast_node_t* n, size_t d) {
  for (size_t i = 0; i < d; i++) {
    mt_log_append(l, "│  ");
  }

  const char* nd_txt = "";

  switch (n->type) {
    case ND_GLOBAL_SCOPE:         nd_txt = "GLOBAL SCOPE"; break;
    case ND_LOCAL_SCOPE:          nd_txt = "LOCAL SCOPE"; break;
    case ND_ENUM_DECL:            nd_txt = "ENUM DECL"; break;
    case ND_ENUM_ITEM:            nd_txt = "ENUM ITEM"; break;
    case ND_STRUCT_DECL:          nd_txt = "STRUCT DECL"; break;
    case ND_FUNC_DECL:            nd_txt = "FUNC DECL"; break;
    case ND_FUNC_DECL_PARAM_LIST: nd_txt = "FUNC DECL PARAM LIST"; break;
    case ND_FUNC_DECL_PARAM:      nd_txt = "FUNC DECL PARAM"; break;
    case ND_FUNC_PARAM:           nd_txt = "FUNC PARAM"; break;
    case ND_FUNC_STORAGE_SPEC:    nd_txt = "FUNC STORAGE SPEC"; break;
    case ND_FUNC_CALL:            nd_txt = "FUNC CALL"; break;
    case ND_FUNC_CALL_PARAM_LIST: nd_txt = "FUNC CALL PARAM LIST"; break;
    case ND_VAR_DECL:             nd_txt = "VAR DECL"; break;
    case ND_VAR_TYPE:             nd_txt = "VAR TYPE"; break;
    case ND_VAR_STORAGE_SPEC:     nd_txt = "VAR STORAGE SPEC"; break;
    case ND_VAR_TYPE_QUAL:        nd_txt = "VAR TYPE QUAL"; break;
    case ND_VAR_VAL:              nd_txt = "VAR VAL"; break;
    case ND_STMT_COMPOUND:        nd_txt = "STMT COMPOUND"; break;
    case ND_STMT_IF:              nd_txt = "STMT IF"; break;
    case ND_STMT_DO_WHILE:        nd_txt = "STMT DO WHILE"; break;
    case ND_STMT_WHILE:           nd_txt = "STMT WHILE"; break;
    case ND_STMT_FOR:             nd_txt = "STMT FOR"; break;
    case ND_STMT_RETURN:          nd_txt = "STMT RETURN"; break;
    case ND_STMT_BREAK:           nd_txt = "STMT BREAK"; break;
    case ND_STMT_CONTINUE:        nd_txt = "STMT CONTINUE"; break;
    case ND_STMT_GOTO:            nd_txt = "STMT GOTO"; break;
    case ND_STMT_LABEL:           nd_txt = "STMT LABEL"; break;
    case ND_STMT_ASSIGN:          nd_txt = "STMT ASSIGN"; break;
    case ND_STMT_MEMBER:          nd_txt = "STMT MEMBER"; break;
    case ND_EXPR:                 nd_txt = "EXPR"; break;
    case ND_ID:                   nd_txt = "ID"; break;
    case ND_INT_LITERAL:          nd_txt = "INT LITERAL"; break;
    case ND_FLOAT_LITERAL:        nd_txt = "FLOAT LITERAL"; break;
    case ND_STRING_LITERAL:       nd_txt = "STRING LITERAL"; break;
    case ND_CHAR_LITERAL:         nd_txt = "CHAR LITERAL"; break;
    case ND_OP_ARITH:
      switch ((mt_operator_t)n->i_val) {
        case OP_ADD:          nd_txt = "+"; break;
        case OP_MIN:          nd_txt = "-"; break;
        case OP_MUL:          nd_txt = "*"; break;
        case OP_DIV:          nd_txt = "/"; break;
        case OP_MOD:          nd_txt = "%"; break;
        case OP_BIT_AND:      nd_txt = "&"; break;
        case OP_BIT_OR:       nd_txt = "|"; break;
        case OP_BIT_XOR:      nd_txt = "^"; break;
        case OP_BIT_LSH:      nd_txt = "<<"; break;
        case OP_BIT_RSH:      nd_txt = ">>"; break;
        default:              nd_txt = "OTHER ARITH OP"; break;
      }
      break;
    case ND_OP_ASSIGN:
      switch ((mt_operator_t)n->i_val) {
        case OP_ASGN:         nd_txt = "="; break;
        case OP_ASGN_ADD:     nd_txt = "+="; break;
        case OP_ASGN_MIN:     nd_txt = "-="; break;
        case OP_ASGN_MUL:     nd_txt = "*="; break;
        case OP_ASGN_DIV:     nd_txt = "/="; break;
        case OP_ASGN_MOD:     nd_txt = "%="; break;
        case OP_ASGN_BIT_AND: nd_txt = "&="; break;
        case OP_ASGN_BIT_OR:  nd_txt = "|="; break;
        case OP_ASGN_BIT_XOR: nd_txt = "^="; break;
        case OP_ASGN_BIT_LSH: nd_txt = "<<="; break;
        case OP_ASGN_BIT_RSH: nd_txt = ">>="; break;
        default:              nd_txt = "OTHER ASGN OP"; break;
      }
      break;
    case ND_OP_LOG:
      switch ((mt_operator_t)n->i_val) {
        case OP_LOG_AND:      nd_txt = "&&"; break;
        case OP_LOG_OR:       nd_txt = "||"; break;
        default:              nd_txt = "OTHER LOG OP"; break;
      }
      break;
    case ND_OP_CMP:
      switch ((mt_operator_t)n->i_val) {
        case OP_CMP_EQ:       nd_txt = "=="; break;
        case OP_CMP_NE:       nd_txt = "!="; break;
        case OP_CMP_GT:       nd_txt = ">"; break;
        case OP_CMP_GE:       nd_txt = ">="; break;
        case OP_CMP_LT:       nd_txt = "<"; break;
        case OP_CMP_LE:       nd_txt = "<="; break;
        default:              nd_txt = "OTHER CMP OP"; break;
      }
      break;

    case ND_OP_PREFIX:
      switch ((mt_operator_t)n->i_val) {
        case OP_LOG_NOT:      nd_txt = "!"; break;
        case OP_BIT_NOT:      nd_txt = "~"; break;
        case OP_REF:          nd_txt = "@"; break;
        case OP_DEREF:        nd_txt = "*"; break;
        case OP_PRE_INC:      nd_txt = "++ (PRE)"; break;
        case OP_PRE_DEC:      nd_txt = "-- (PRE)"; break;
        case OP_POS:          nd_txt = "+ (POS)"; break;
        case OP_NEG:          nd_txt = "- (NEG)"; break;
        default:              nd_txt = "OTHER PREFIX OP"; break;
      }
      break;
    case ND_OP_SUFFIX:
      switch ((mt_operator_t)n->i_val) {
        case OP_MEMBER:       nd_txt = "."; break;
        case OP_ARRAY:        nd_txt = "[]"; break;
        case OP_POST_INC:     nd_txt = "++ (POST)"; break;
        case OP_POST_DEC:     nd_txt = "-- (POST)"; break;
        default:              nd_txt = "OTHER SUFFIX OP"; break;
      }
      break;
    case ND_LABEL:                nd_txt = "LABEL"; break;
  }

  mt_log_append(l, "├─ %s\n", nd_txt);

  for (size_t i = 0; i < l_size(n->sub); i++) {
    mt_ast_node_dump(l, l_at(n->sub, i), d + 1);
  }
}