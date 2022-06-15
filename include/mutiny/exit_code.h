#ifndef __MT_EXIT_CODE_H__
#define __MT_EXIT_CODE_H__

/**
 * @brief The exit codes for the compiler.
 */
typedef enum mt_exit_code {
  MT_EXIT_ERR_INTERNAL = -1,
  MT_EXIT_SUCCEESS     = 0,
  MT_EXIT_ERR_ARGS     = 1,
  MT_EXIT_ERR_FILE     = 2,
  MT_EXIT_ERR_SYNTAX   = 3,
  MT_EXIT_ERR_OS       = 4,
  MT_EXIT_ERR_ARCH     = 5,
} mt_exit_code_t;

#endif // __MT_EXIT_CODE_H__
