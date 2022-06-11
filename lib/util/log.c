#include <mutiny/util/log.h>
#include <mutiny/mutiny.h>

mt_log_t mt_log_init(FILE* f, const char* t) {
  return (mt_log_t){ f, t, "" };
}

void mt_log_add(mt_log_t* l, const char* f, ...) {
  va_list args;
  va_start(args, f);
  
  if (strlen(l->start_txt)) {
    sprintf(l->buf + strlen(l->buf), "%s", l->start_txt);
  }
  vsprintf(l->buf + strlen(l->buf), f, args);
  
  va_end(args);
}

void mt_log_dump(mt_log_t* l) {
  fputs(l->buf, l->file);
  l->buf[0] = '\0';
}
