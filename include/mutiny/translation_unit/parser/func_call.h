#ifndef __MT_PARSER_FUNC_CALL_H__
#define __MT_PARSER_FUNC_CALL_H__

struct _mt_ast_node;
struct _mt_token;
struct _mt_error_reporter;

/**
 * @brief Parses a function call.
 * @param token The current token to parse.
 * @param error_reporter The translation unit's error reporter.
 * @return An AST representing the function call.
 */
struct _mt_ast_node* mt_parse_func_call(struct _mt_token** token, struct _mt_error_reporter* error_reporter);

#endif // __MT_PARSER_FUNC_CALL_H__