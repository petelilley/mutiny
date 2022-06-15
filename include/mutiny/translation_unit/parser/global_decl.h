#ifndef __MT_GLOBAL_DECL_H__
#define __MT_GLOBAL_DECL_H__

struct _mt_ast_node;
struct _mt_token;

struct _mt_ast_node* mt_parse_global_decl(struct _mt_token** token);

#endif // __MT_GLOBAL_DECL_H__