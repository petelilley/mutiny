#ifndef __MT_AST_H__
#define __MT_AST_H__

/**
 * @brief The type of an AST node.
 */
typedef enum _mt_ast_type {
  AST_IDENTIFIER = 0,
} mt_ast_type_t;

/**
 * @brief The value of an AST node.
 */
typedef struct _mt_ast_val {

} mt_ast_val_t;

#include <mutiny/util/list.h>

/**
 * @brief Abstract syntax tree node that represents the syntax of mutiny source code.
 */
typedef struct _mt_ast {
  // The type of the AST node.
  mt_ast_type_t type;
  
  // The value of the AST node.
  mt_ast_val_t* val;
  
  // Lower AST nodes in the tree.
  list_t(struct _mt_ast*) sub;
} mt_ast_t;

#endif // __MT_AST_H__
