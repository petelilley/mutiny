#include <mutiny/mutiny.h>
#include <mutiny/error/error.h>
#include <mutiny/util/filesystem.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

// TODO Make file handling cross-platform.

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

mt_file_t* file_open(const char* path) {
  if (!is_file(path)) {
    fprintf(stderr, MT_ERROR "no such file '%s'\n", path);
    return NULL;
  }
  int fd;
  if ((fd = open(path, O_RDONLY) == -1)) {
    fprintf(stderr, MT_ERROR "failed to open file '%s': %s\n", path, strerror(errno));
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
