#ifndef __MT_ERROR_H__
#define __MT_ERROR_H__

#include <mutiny/mutiny.h>
#include <mutiny/util/log.h>

/**
 * @brief An error manager that holds information about runtime compiler errors.
 */
typedef struct _mt_error_reporter {
  mt_log_t err_log;
  mt_log_t warn_log;
  size_t err_num;
  ssize_t err_max;
  size_t warn_num;
  ssize_t warn_max;
} mt_error_reporter_t;

/**
 * @brief Creates a new error reporter.
 * @return A new error reporter.
 */
static inline mt_error_reporter_t mt_error_reporter_init() {
  return (mt_error_reporter_t){
    .err_log = mt_log_init(stderr, MT_ERROR),
    .warn_log = mt_log_init(stderr, MT_WARNING),
    .err_num = 0,
    .err_max = -1,
    .warn_num = 0,
    .warn_max = -1,
  };
}

#endif // __MT_ERROR_H__