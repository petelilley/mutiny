#ifndef __MT_PARSER_STMT_H__
#define __MT_PARSER_STMT_H__

struct _mt_ast_node;
struct _mt_token;

/**
 * @brief Parses a statement.
 * @param token The current token to parse.
 * @return An AST representing the statement.
 */
struct _mt_ast_node* mt_parse_stmt(struct _mt_token** token);

/**
 * @brief Parses a compound statement.
 * @param token The current token to parse.
 * @return An AST representing the compound statement.
 */
struct _mt_ast_node* mt_parse_compound_stmt(struct _mt_token** token);

#endif // __MT_PARSER_STMT_H__