#include <mutiny/mutiny.h>
#include <mutiny/util/filesystem.h>
#include <mutiny/util/log.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

// TODO: Make file handling cross-platform.

bool is_file(const char* path) {
  struct stat s;
  if (stat(path, &s) == -1) return false;
  return s.st_mode & S_IFREG;
}

bool is_directory(const char* path) {
  struct stat s;
  if (stat(path, &s) == -1) return false;
  return s.st_mode & S_IFDIR;
}

mt_file_t* mt_file_init(const char* path) {
  mt_log_t err_log = mt_log_init(stderr, MT_ERROR);

  FILE* fp = fopen(path, "r");
  if (!fp) {
    mt_log_add(&err_log, "No such file '%s'\n", path);
    mt_log_dump(&err_log);
    return NULL;
  }
  
  mt_file_t* f = malloc(sizeof(mt_file_t));
  
  fseek(fp, 0, SEEK_END);
  size_t length = ftell(fp);
  
  fseek(fp, 0, SEEK_SET);
  
  f->contents =  malloc(length + sizeof(char));
  fread(f->contents, sizeof(char), length, fp);
  f->contents[length] = 0;
  
  fclose(fp);
  
  f->path = strdup(path);
  f->ptr = f->contents;
  f->cur_line = f->cur_col = 1;
  f->cur_pos = 0;

  printf("file: '%s'\n", f->contents);
  
  return f;
}

void mt_file_deinit(mt_file_t* f) {
  if (!f) return;
  
  if (f->path) {
    free(f->path);
  }
  if (f->contents) {
    free(f->contents);
  }
  
  free(f);
}

static bool is_newline(mt_file_t* f) {
  char c = *f->ptr;
  if (c == '\n') return true;
  if (c != '\r') return false;
  if (*(f->ptr + 1) == '\n') {
    ++f->cur_col;
    ++f->cur_pos;
    ++f->ptr;
  }
  return true;
}

char mt_file_getc(mt_file_t* f) {
  if (is_newline(f)) {
    ++f->cur_line;
    f->cur_col = 1;
  }
  else {
    ++f->cur_col;
  }
  
  ++f->cur_pos;
  return *++f->ptr;
}

char mt_file_skip(mt_file_t* f, size_t n) {
  char last = 0;
  for (size_t i = 0; i < n; i++) {
    last = mt_file_getc(f);
  }
  return last;
}