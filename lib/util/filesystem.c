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
  
  if (!is_file(path)) {
    mt_log_add(&err_log, "No such file '%s'\n", path);
    mt_log_dump(&err_log);
    return NULL;
  }
  int fd;
  if ((fd = open(path, O_RDONLY)) == -1) {
    mt_log_add(&err_log, "Failed to open file '%s': %s\n", path, strerror(errno));
    mt_log_dump(&err_log);
    return NULL;
  }
  
  struct stat s;
  stat(path, &s);
  
  size_t size = s.st_size;
  
  mt_file_t* f = malloc(sizeof(mt_file_t));
  f->contents = malloc(size);
  
  read(fd, f->contents, size);
  
  close(fd);
  
  f->path = strdup(path);
  f->ptr = f->contents;
  f->cur_line = f->cur_col = 1;
  f->cur_pos = 0;
  
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
  for (size_t i = 0; i < n; i++) {
    mt_file_getc(f);
  }
}