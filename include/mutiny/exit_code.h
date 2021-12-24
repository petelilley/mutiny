#ifndef __MT_EXIT_CODE_H__
#define __MT_EXIT_CODE_H__

/**
 * @brief An enumeration of the compiler's possible exit codes.
 */
typedef enum mt_exit_code {
  EXIT_ERR_INTERNAL = -1,
  EXIT_SUCCEEDED    = 0,
  EXIT_ERR_ARGS     = 1,
  EXIT_ERR_FILE     = 2,
  EXIT_ERR_SYNTAX   = 3,
  EXIT_ERR_OS       = 4,
  EXIT_ERR_ARCH     = 5,
} mt_exit_code_t;

#endif // __MT_EXIT_CODE_H__
