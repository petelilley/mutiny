#ifndef __MT_PARSER_GLOBAL_DECL_H__
#define __MT_PARSER_GLOBAL_DECL_H__

struct _mt_ast_node;
struct _mt_token;

/**
  * @brief Parses a global declaration.
  * @param token The current token to parse.
  * @return An AST representing the global declaration.
  */
struct _mt_ast_node* mt_parse_global_decl(struct _mt_token** token);

#endif // __MT_PARSER_GLOBAL_DECL_H__