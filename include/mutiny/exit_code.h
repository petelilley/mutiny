#ifndef __MT_EXIT_CODE_H__
#define __MT_EXIT_CODE_H__

/**
 * @brief An enumeration of the compiler's possible exit codes.
 */
typedef enum mt_exit_code {
  exit_err_internal = -1,
  exit_success      = 0,
  exit_err_args     = 1,
  exit_err_file     = 2,
  exit_err_syntax   = 3,
  exit_err_os       = 4,
  exit_err_arch     = 5,
} mt_exit_code_t;

#endif // __MT_EXIT_CODE_H__
