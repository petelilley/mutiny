#include <mutiny/settings.h>
#include <mutiny/mutiny.h>
#include <mutiny/exit_code.h>
#include <mutiny/util/list.h>

mt_settings_t* mt_settings_init() {
  mt_settings_t* s = malloc(sizeof(mt_settings_t));
  memset(s, 0, sizeof(mt_settings_t));
  
  s->exit_code = MT_EXIT_SUCCEESS;
  
  l_init(s->import_paths);
  l_init(s->lib_paths);
  l_init(s->link_libs);
  l_init(s->src_dirs);
  
  return s;
}

void mt_settings_deinit(mt_settings_t* s) {
  if (!s) return;
  
  for (size_t i = 0; i < l_size(s->import_paths); i++) {
    free(l_at(s->import_paths, i));
  }
  l_deinit(s->import_paths);
  
  for (size_t i = 0; i < l_size(s->lib_paths); i++) {
    free(l_at(s->lib_paths, i));
  }
  l_deinit(s->lib_paths);
  
  for (size_t i = 0; i < l_size(s->link_libs); i++) {
    free(l_at(s->link_libs, i));
  }
  l_deinit(s->link_libs);
  
  for (size_t i = 0; i < l_size(s->src_dirs); i++) {
    free(l_at(s->src_dirs, i));
  }
  l_deinit(s->src_dirs);
  
  if (s->output) {
    free(s->output);
  }
  
  free(s);
}
