#include <mutiny/parser/token.h>
#include <mutiny/mutiny.h>
#include <mutiny/util/filesystem.h>

mt_token_t* mt_token_init(struct _mt_file* f) {
  mt_token_t* t = malloc(sizeof(mt_token_t));
  memset(t, 0, sizeof(mt_token_t));
  
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
