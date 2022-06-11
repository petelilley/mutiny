#ifndef __MT_TIMER_H__
#define __MT_TIMER_H__

#include <mutiny/mutiny.h>

typedef struct _mt_timer {
  clock_t start_time;
} mt_timer_t;

mt_timer_t mt_timer_init();
double mt_timer_get(mt_timer_t* timer);

#endif // __MT_TIMER_H__
