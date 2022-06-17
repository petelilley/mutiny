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

ssize_t mt_str_to_keyword(const char* s, size_t l) {
  for (size_t i = 1; i < sizeof(kw_strs) / sizeof(char*); ++i) {
    if (!strncmp(s, kw_strs[i], l)) {
      return i;
    }
  }
  return -1;
}

static const char* kind_strs[] = {
  [TK_IDENTIFIER] = "identifier",
  [TK_KEYWORD]    = "keyword",
  [TK_PUNCTUATOR] = "punctuator",
  [TK_INTEGER]    = "integer",
  [TK_FLOAT]      = "float",
  [TK_STRING]     = "string",
  [TK_CHAR]       = "char",
  [TK_EOF]        = "EOF",
};

const char* mt_token_kind_to_str(mt_token_kind_t k) {
  return kind_strs[k];
}

static const char* punct_strs[] = {
  [PCT_LPAREN]     = "(",
  [PCT_RPAREN]     = ")",
  [PCT_LBRACE]     = "[",
  [PCT_RBRACE]     = "]",
  [PCT_LBRACKET]   = "{",
  [PCT_RBRACKET]   = "}",
  [PCT_COMMA]      = ",",
  [PCT_SEMICOLON]  = ";",
  [PCT_COLON]      = ":",
  [PCT_DOT]        = ".",
  [PCT_EQ]         = "=",
  [PCT_PLUS]       = "+",
  [PCT_MINUS]      = "-",
  [PCT_MUL]        = "*",
  [PCT_DIV]        = "/",
  [PCT_MOD]        = "%",
  [PCT_BIT_AND]    = "&",
  [PCT_BIT_OR]     = "|",
  [PCT_BIT_XOR]    = "^",
  [PCT_BIT_NOT]    = "~",
  [PCT_BIT_LSH]    = "<<",
  [PCT_BIT_RSH]    = ">>",
  [PCT_PLUS_EQ]    = "+=",
  [PCT_MINUS_EQ]   = "-=",
  [PCT_MUL_EQ]     = "*=",
  [PCT_DIV_EQ]     = "/=",
  [PCT_MOD_EQ]     = "%=",
  [PCT_BIT_AND_EQ] = "&=",
  [PCT_BIT_OR_EQ]  = "|=",
  [PCT_BIT_XOR_EQ] = "^=",
  [PCT_LSH_EQ]     = "<<=",
  [PCT_RSH_EQ]     = ">>=",
  [PCT_EQ_EQ]      = "==",
  [PCT_NOT_EQ]     = "!=",
  [PCT_LESS_EQ]    = "<=",
  [PCT_GREATER_EQ] = ">=",
  [PCT_LOG_OR]     = "||",
  [PCT_LOG_AND]    = "&&",
  [PCT_INC]        = "++",
  [PCT_DEC]        = "--",
  [PCT_ARROW]      = "->",
  [PCT_GREATER]    = ">",
  [PCT_LESS]       = "<",
  [PCT_NOT]        = "!",
  [PCT_REF]        = "@",
  [PCT_COL_COL]    = "::",
  [PCT_QUESTION]   = "?",
  [PCT_COL_EQ]     = ":=",
};

const char* mt_punct_to_str(mt_punctuator_t p) {
  return punct_strs[p];
}

ssize_t mt_str_to_punct(const char* s, size_t l) {
  for (size_t i = 1; i < sizeof(punct_strs) / sizeof(char*); ++i) {
    if (!strncmp(s, punct_strs[i], l)) {
      return i;
    }
  }
  return -1;
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
    
    if (tmp->str_val) {
      free(tmp->str_val);
    }
    free(tmp);
  }
}
