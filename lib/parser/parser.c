#include <mutiny/mutiny.h>
#include <mutiny/parser/parser.h>
#include <mutiny/parser/token.h>
#include <mutiny/parser/translation_unit.h>
#include <mutiny/ast/ast.h>
#include <mutiny/settings.h>
#include <mutiny/util/list.h>

/*
enum {
  SCS_STATIC = 1 << 0,
};
typedef unsigned mt_storage_class_spec_t;

enum {
  TQ_CONST    = 1 << 0,
  TQ_VOLATILE = 1 << 1,
};
typedef unsigned mt_type_qual_t;
*/

static mt_ast_node_t* global_decl(mt_token_t** token);
static mt_ast_node_t* enum_decl(mt_token_t** token);
static mt_ast_node_t* struct_decl(mt_token_t** token);
static mt_ast_node_t* func_decl(mt_token_t** token);
static mt_ast_node_t* func_decl_param_list(mt_token_t** token);
static mt_ast_node_t* var_decl(mt_token_t** token);

bool mt_translation_unit_parse_tokens(mt_translation_unit_t* t_unit) {
  t_unit->ast = mt_ast_node_init(ND_GLOBAL_SCOPE);
  
  mt_token_t* tok = t_unit->tokens;
  mt_ast_node_t* node;
  while (tok->kind != TK_EOF) {
    node = global_decl(&tok);
    if (!node) {
      // TODO: Error.
      break;
    }
    l_push(t_unit->ast->sub, mt_ast_node_t*, node);
  }
  
  return true;
}

static mt_ast_node_t* global_decl(mt_token_t** t) {
  // TODO import, enum, struct, union, function, type alias, global variable
  if ((*t)->kind == TK_KEYWORD) {
    switch ((mt_keyword_t)(*t)->ival) {
      case KW_ENUM:
        return enum_decl(t);
      case KW_STRUCT:
        return struct_decl(t);
      case KW_FUNC:
        return func_decl(t);
      case KW_VAR:
        return var_decl(t);
      default:
        // TODO: Error.
        break;
    }
  }
  else {
  }
    return NULL;
}

static inline bool comp_tok(mt_token_t* t, mt_token_kind_t k) {
  return t->kind == k;
}

static inline bool comp_kw(mt_token_t* t, mt_keyword_t k) {
  return comp_tok(t, TK_KEYWORD) && t->ival == k;
}

static inline bool comp_punc(mt_token_t* t, const char* punc) {
  return comp_tok(t, TK_PUNCTUATOR) && !strcmp(t->strval, punc);
}

static bool match(mt_token_t* t, mt_token_kind_t k) {
  if (comp_tok(t, k)) {
    return true;
  }
  // TODO: Error.
  return false;
}

static bool match_kw(mt_token_t* t, mt_keyword_t k) {
  if (comp_kw(t, k)) {
    return true;
  }
  // TODO: Error.
  return false;
}

static bool match_punc(mt_token_t* t, const char* punc) {
  if (comp_punc(t, punc)) {
    return true;
  }
  // TODO: Error.
  return false;
}

static mt_ast_node_t* enum_decl(mt_token_t** _t) {
  mt_token_t* t = *_t;
  
  return NULL;
}

static mt_ast_node_t* struct_decl(mt_token_t** _t) {
  mt_token_t* t = *_t;
  
  return NULL;
}

static mt_ast_node_t* func_decl(mt_token_t** _t) {
  mt_token_t* t = *_t;
  
  mt_ast_node_t* param_list_nd;
  
  do {
    if (!match_kw(t, KW_FUNC)) break;
    t = t->next;
    
    if (!match(t, TK_IDENTIFIER)) break;
    const char* name = t->strval;
    t = t->next;
    
    if (!match_punc(t, ":")) break;
    t = t->next;
    
    if (!match_punc(t, "(")) break;
    t = t->next;
    
    param_list_nd = func_decl_param_list(&t);
    
    if (!match_punc(t, ")")) break;
    t = t->next;
    
    // Return type.
    const char* ret_type = "void";
    
    if (comp_punc(t, "->")) {
      t = t->next;
      
      if (!match(t, TK_IDENTIFIER)) break;
      ret_type = t->strval;
      t = t->next;
    }
    
    printf("function\n\tname: %s\n\treturns: %s\n", name, ret_type);
  
    if (param_list_nd) {
      for (size_t i = 0; i < l_size(param_list_nd->sub); i++) {
        printf("\tparam name: %s, type: %s\n", l_at(l_at(param_list_nd->sub, i)->sub, 0)->strval, l_at(l_at(param_list_nd->sub, i)->sub, 1)->strval);
      }
    }
    
  } while (0);

  *_t = t;
  
  return NULL;
}

static mt_ast_node_t* func_decl_param_list(mt_token_t** _t) {
  mt_token_t* t = *_t;
  
  if (match_punc(t, ")")) return NULL;
  
  mt_ast_node_t* param_list_nd = NULL;
  mt_ast_node_t* param_nd, *name_nd, *type_nd;
  
  do {
    if (!match(t, TK_IDENTIFIER)) break;
    char* name = t->strval;
    t = t->next;
    
    if (!match_punc(t, ":")) break;
    t = t->next;
    
    // TODO: Dedicated type parser.
    
    if (!match(t, TK_IDENTIFIER)) break;
    char* type = t->strval;
    t = t->next;
    
    if (!param_list_nd) {
      param_list_nd = mt_ast_node_init(ND_FUNC_DECL_PARAM_LIST);
    }
    
    name_nd = mt_ast_node_init(ND_VAR_NAME);
    name_nd->strval = name;
    
    type_nd = mt_ast_node_init(ND_VAR_TYPE);
    type_nd->strval = type;
    
    param_nd = mt_ast_node_init(ND_FUNC_DECL_PARAM);
    l_push(param_nd->sub, mt_ast_node_t*, name_nd);
    l_push(param_nd->sub, mt_ast_node_t*, type_nd);
    l_push(param_list_nd->sub, mt_ast_node_t*, param_nd);
    
    if (comp_punc(t, ")")) break;
    
    if (!match_punc(t, ",")) break;
    t = t->next;
  } while (1);
  
  *_t = t;
  
  return param_list_nd;
}

static mt_ast_node_t* var_decl(mt_token_t** t) {
  return NULL;
}
