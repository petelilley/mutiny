#ifndef __MT_TIMER_H__
#define __MT_TIMER_H__

#include <mutiny/mutiny.h>

/**
 * @brief Represents a timer.
 */
typedef struct _mt_timer {
  clock_t start_time;
} mt_timer_t;

/**
 * @brief Creates a new timer.
 * @return A new timer.
 */
mt_timer_t mt_timer_init();

/**
 * @brief Resets a timer to 0s.
 * @param timer The timer to reset.
 */
void mt_timer_reset(mt_timer_t* timer);

/**
 * @brief Returns the time elapsed since the timer was initialized.
 * @param timer The timer.
 * @return The time elapsed since the timer was initialized.
 */
double mt_timer_get(mt_timer_t* timer);

#endif // __MT_TIMER_H__
