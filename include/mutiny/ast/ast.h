#ifndef __MT_AST_H__
#define __MT_AST_H__

#include <mutiny/mutiny.h>
#include <mutiny/util/list.h>
#include <mutiny/translation_unit/lexer/token.h>

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
  ND_VAR_TYPE,              // Variable type.
  ND_VAR_STORAGE_SPEC,      // Variable storage specifier.
  ND_VAR_TYPE_QUAL,         // Variable type qualifier.
  ND_VAR_VAL,               // Variable value.
  
  ND_STMT_COMPOUND,         // Compound statement.
  ND_STMT_IF,               // If statement.
  ND_STMT_DO_WHILE,         // Do-while statement.
  ND_STMT_WHILE,            // While statement.
  ND_STMT_FOR,              // For statement.
  ND_STMT_RETURN,           // Return statement.
  ND_STMT_BREAK,            // Break statement.
  ND_STMT_CONTINUE,         // Continue statement.
  ND_STMT_GOTO,             // Goto statement.
  ND_STMT_LABEL,            // Label statement.
  ND_STMT_ASSIGN,           // Variable assignment statement.
  ND_STMT_MEMBER,           // Member access statement.
  
  ND_EXPR,                  // Expression.

  ND_ID,                    // Identifier.
  ND_INT_LITERAL,           // Integer literal.
  ND_FLOAT_LITERAL,         // Floating-point literal.
  ND_STRING_LITERAL,        // String literal.
  ND_CHAR_LITERAL,          // Character literal.

  ND_OP_ARITH,              // Arithmetic operator.
  ND_OP_ASSIGN,             // Assignment operator.
  ND_OP_LOG,                // Logical operator.
  ND_OP_CMP,                // Comparison operator.
  
  ND_LABEL,                 // Label.
} mt_ast_type_t;

typedef enum _mt_operator {
  OP_UNKNOWN = 0,
  OP_ADD,          // +
  OP_MIN,          // -
  OP_MUL,          // *
  OP_DIV,          // /
  OP_MOD,          // %
  OP_BIT_AND,      // &
  OP_BIT_OR,       // |
  OP_BIT_XOR,      // ^
  OP_BIT_NOT,      // ~
  OP_BIT_LSH,      // <<
  OP_BIT_RSH,      // >>
  OP_ASGN,         // =
  OP_ASGN_ADD,     // +=
  OP_ASGN_MIN,     // -=
  OP_ASGN_MUL,     // *=
  OP_ASGN_DIV,     // /=
  OP_ASGN_MOD,     // %=
  OP_ASGN_BIT_AND, // &=
  OP_ASGN_BIT_OR,  // |=
  OP_ASGN_BIT_XOR, // ^=
  OP_ASGN_BIT_LSH, // <<=
  OP_ASGN_BIT_RSH, // >>=
  OP_CMP_EQ,       // ==
  OP_CMP_NE,       // !=
  OP_CMP_GT,       // >
  OP_CMP_GE,       // >=
  OP_CMP_LT,       // <
  OP_CMP_LE,       // <=
  OP_LOG_AND,      // &&
  OP_LOG_OR,       // ||
  OP_LOG_NOT,      // !
  OP_REF,          // @
  OP_DEREF,        // *
  OP_MEMBER,       // .
  OP_ARRAY,        // []
  OP_PRE_INC,      // ++ before
  OP_PRE_DEC,      // -- before
  OP_POST_INC,     // ++ after
  OP_POST_DEC,     // -- after
  OP_POS,          // + before
  OP_NEG,          // - before
} mt_operator_t;

/**
 * @brief Converts a punctuator to an operator.
 * @param punct The punctuator to convert.
 * @return The operator, or OP_UNKNOWN if the punctuator is not an operator.
 */
mt_operator_t mt_punct_to_operator(mt_punctuator_t punct);

#define MT_OP_PREC_HIGHEST 0
#define MT_OP_PREC_LOWEST 12

/**
 * @brief Returns an operator's precedence.
 * @param op The operator.
 * @return The operator's precedence.
 */
size_t mt_get_operator_precedence(mt_operator_t op);

/**
 * @brief An AST node.
 */
typedef struct _mt_ast_node {
  mt_ast_type_t type;
  
  char c_val;
  long long int i_val;
  long double f_val;
  char* str_val;
  
  list_t(struct _mt_ast_node*) sub;
} mt_ast_node_t;

/**
 * @brief Creates a new AST node.
 * @param type The type of the node.
 * @return A new AST node.
 */
mt_ast_node_t* mt_ast_node_init(mt_ast_type_t type);

/**
 * Destroys an AST node.
 * @param node The node to destroy.
 */
void mt_ast_node_deinit(mt_ast_node_t* ast);

#endif // __MT_AST_H__
