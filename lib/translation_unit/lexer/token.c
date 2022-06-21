#include <mutiny/translation_unit/lexer/token.h>
#include <mutiny/mutiny.h>
#include <mutiny/util/filesystem.h>

static const char* kw_strs[] = {
  [KW_IMPORT]   = "import",
  
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

ssize_t mt_str_to_keyword(const char* s, size_t l) {
  for (size_t i = 1; i < sizeof(kw_strs) / sizeof(char*); ++i) {
    const char* kw = kw_strs[i];
    if (strlen(kw) == l && !strncmp(s, kw, l)) {
      return i;
    }
  }
  return -1;
}

static const char* kind_strs[] = {
  [TK_IDENTIFIER] = "identifier",
  [TK_KEYWORD]    = "keyword",
  [TK_PUNCTUATOR] = "punctuator",
  [TK_NUMBER]     = "number",
  [TK_STRING]     = "string",
  [TK_CHAR]       = "char",
  [TK_EOF]        = "EOF",
};

const char* mt_token_kind_to_str(mt_token_kind_t k) {
  const char* s = kind_strs[k];
  return s;
}

static const char* punct_strs[] = {
  [PCT_UNKNOWN]         = NULL,
  [PCT_LPAR]            = "(",
  [PCT_RPAR]            = ")",
  [PCT_LBRACE]          = "{",
  [PCT_RBRACE]          = "}",
  [PCT_LBRACKET]        = "[",
  [PCT_RBRACKET]        = "]",
  [PCT_COMMA]           = ",",
  [PCT_SC]              = ";",
  [PCT_COL]             = ":",
  [PCT_DOT]             = ".",
  [PCT_ASGN]              = "=",
  [PCT_ADD]             = "+",
  [PCT_MIN]             = "-",
  [PCT_MUL]             = "*",
  [PCT_DIV]             = "/",
  [PCT_MOD]             = "%",
  [PCT_BIT_AND]         = "&",
  [PCT_BIT_OR]          = "|",
  [PCT_BIT_XOR]         = "^",
  [PCT_BIT_NOT]         = "~",
  [PCT_CMP_GT]          = ">",
  [PCT_CMP_LT]          = "<",
  [PCT_LOG_NOT]         = "!",
  [PCT_REF]             = "@",
  [PCT_QUE]             = "?",
  [PCT_BIT_LSH]         = "<<",
  [PCT_BIT_RSH]         = ">>",
  [PCT_ASGN_ADD]     = "+=",
  [PCT_ASGN_MIN]     = "-=",
  [PCT_ASGN_MUL]     = "*=",
  [PCT_ASGN_DIV]     = "/=",
  [PCT_ASGN_MOD]     = "%=",
  [PCT_ASGN_BIT_AND] = "&=",
  [PCT_ASGN_BIT_OR]  = "|=",
  [PCT_ASGN_BIT_XOR] = "^=",
  [PCT_ASGN_BIT_LSH] = "<<=",
  [PCT_ASGN_BIT_RSH] = ">>=",
  [PCT_CMP_EQ]          = "==",
  [PCT_CMP_NE]         = "!=",
  [PCT_CMP_LE]         = "<=",
  [PCT_CMP_GE]         = ">=",
  [PCT_LOG_OR]          = "||",
  [PCT_LOG_AND]         = "&&",
  [PCT_INC]             = "++",
  [PCT_DEC]             = "--",
  [PCT_ARROW]           = "->",
  [PCT_COL_COL]         = "::",
  [PCT_COL_EQ]          = ":=",
};

const char* mt_punct_to_str(mt_punctuator_t p) {
  return punct_strs[p];
}

ssize_t mt_str_to_punct(const char* s, size_t l) {
  if (!l) return -1;
  
  if (l == 1) return *s;
  
  for (size_t i = 256; i < sizeof(punct_strs) / sizeof(char*); ++i) {
    if (!strncmp(s, punct_strs[i], l)) {
      return i;
    }
  }
  return -1;
}

mt_token_t* mt_token_init(struct _mt_file* f) {
  mt_token_t* t = malloc(sizeof(mt_token_t));
  memset(t, 0, sizeof(mt_token_t));

  t->i_val = MT_UNDEFINED;
  t->f_val = MT_UNDEFINED;
  
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
    
    if (tmp->str_val) {
      free(tmp->str_val);
    }
    free(tmp);
  }
}
