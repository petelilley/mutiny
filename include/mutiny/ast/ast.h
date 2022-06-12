#ifndef __MT_AST_H__
#define __MT_AST_H__

#include <mutiny/mutiny.h>
#include <mutiny/util/list.h>

/**
 * @brief The type of an AST node.
 */
typedef enum _mt_ast_node_type {
  ND_GLOBAL_SCOPE = 0,      // Global scope.
  ND_LOCAL_SCOPE,           // Local scope.
  
  ND_ENUM_DECL,             // Enum declaration.
  ND_ENUM_ITEM,             // Enum item.
  
  ND_STRUCT_DECL,           // Struct declaration.
  
  ND_FUNC_DECL,             // Function declaration.
  ND_FUNC_DECL_PARAM_LIST,  // Function declaration parameter list.
  ND_FUNC_DECL_PARAM,       // Function declaration parameter.
  ND_FUNC_PARAM,            // Function parameter.
  ND_FUNC_STORAGE_SPEC,     // Function storage specifier.
  
  ND_FUNC_CALL,             // Function call.
  ND_FUNC_CALL_PARAM_LIST,  // Function call parameter list.
  
  ND_VAR_DECL,              // Variable declaration.
  ND_VAR_NAME,              // Variable type.
  ND_VAR_TYPE,              // Variable type.
  ND_VAR_STORAGE_SPEC,      // Variable storage specifier.
  ND_VAR_TYPE_QUAL,         // Variable type qualifier.
  ND_VAR_VAL,               // Variable value.
  
  ND_STMT_IF,               // If statement.
  ND_STMT_DO_WHILE,         // Do-while statement.
  ND_STMT_WHILE,            // While statement.
  ND_STMT_FOR,              // For statement.
  ND_STMT_RETURN,           // Return statement.
  ND_STMT_GOTO,             // Goto statement.
  ND_STMT_LABEL,            // Label statement.
  ND_STMT_ASSIGN,           // Variable assignment statement.
  ND_STMT_MEMBER,           // Member access statement.
  
  ND_EXPR_CONST,            // Constant expression.
  ND_EXPR_EVAL,             // Evaluated expression.
  
  ND_OP_COMP,               // Comparison operator.
  ND_OP_ARITH,              // Arithmetic operator.
  ND_OP_BIT,                // Bitwise operator.
} mt_ast_type_t;

/**
 * @brief Abstract syntax tree node that represents the syntax of mutiny source code.
 */
typedef struct _mt_ast_node {
  mt_ast_type_t type;
  
  char cval;
  long long int ival;
  long double fval;
  char* strval;
  
  list_t(struct _mt_ast_node*) sub;
} mt_ast_node_t;

mt_ast_node_t* mt_ast_node_init(mt_ast_type_t type);

void mt_ast_node_deinit(mt_ast_node_t* ast);

#endif // __MT_AST_H__
