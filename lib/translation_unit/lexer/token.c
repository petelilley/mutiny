#include <mutiny/translation_unit/lexer/token.h>
#include <mutiny/mutiny.h>
#include <mutiny/util/filesystem.h>

static const char* kw_strs[] = {
  [KW_IMPORT]   = "import",
  
  [KW_NOT]      = "not",
  [KW_AND]      = "and",
  [KW_OR]       = "or",
  
  [KW_ENUM]     = "enum",
  [KW_STRUCT]   = "struct",
  [KW_FUNC]     = "func",
  [KW_VAR]      = "var",
  
  [KW_IF]       = "if",
  [KW_ELIF]     = "elif",
  [KW_ELSE]     = "else",
  [KW_WHILE]    = "while",
  [KW_FOR]      = "for",
  [KW_SWITCH]   = "switch",
  [KW_CASE]     = "case",
  [KW_DEFAULT]  = "default",
  [KW_GOTO]     = "goto",
  
  [KW_RETURN]   = "return",
  [KW_BREAK]    = "break",
  [KW_CONTINUE] = "continue",
  
  /*
  [KW_STATIC]   = "static",
  
  [KW_CONST]    = "const",
  [KW_VOLATILE] = "volatile",
  
  [KW_INLINE]   = "inline",
  */
  
  [KW_SIGNED]   = "signed",
  [KW_UNSIGNED] = "unsigned",
};

const char* mt_keyword_to_str(mt_keyword_t k) {
  return kw_strs[k];
}

int mt_str_to_keyword(const char* s, size_t l) {
  for (size_t i = 0; i < sizeof(kw_strs) / sizeof(char*); ++i) {
    if (!strncmp(s, kw_strs[i], l)) {
      return i;
    }
  }
  return -1;
}

const char* mt_token_kind_to_str(mt_token_kind_t k) {
  switch (k) {
    case TK_IDENTIFIER:
      return "identifier";
    case TK_KEYWORD:
      return "keyword";
    case TK_PUNCTUATOR:
      return "punctuator";
    case TK_INTEGER:
      return "integer";
    case TK_FLOAT:
      return "float";
    case TK_STRING:
      return "string";
    case TK_CHAR:
      return "char";
    case TK_EOF:
      return "EOF";
    default:
      return "unknown token";
  }
}

mt_token_t* mt_token_init(struct _mt_file* f) {
  mt_token_t* t = malloc(sizeof(mt_token_t));
  memset(t, 0, sizeof(mt_token_t));
  
  t->file = f;
  t->line = f->cur_line;
  t->col = f->cur_col;
  t->fpos = f->cur_pos;
  
  return t;
}

void mt_token_deinit(mt_token_t* t) {
  mt_token_t* tmp;
  
  while (t) {
    tmp = t;
    t = t->next;
    
    if (tmp->strval) {
      free(tmp->strval);
    }
    free(tmp);
  }
}
