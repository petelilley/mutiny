#include <mutiny/util/timer.h>

mt_timer_t mt_timer_init() {
  return (mt_timer_t){ clock() };
}

double mt_timer_get(mt_timer_t* t) {
  return (double)(clock() - t->start_time) / CLOCKS_PER_SEC;
}
