#ifndef __MT_FILESYSTEM_H__
#define __MT_FILESYSTEM_H__

#include <mutiny/mutiny.h>

/**
 * @brief Represents a file for reading.
 */
typedef struct _mt_file {
  char* path;
  char* contents;
  char* ptr;
  long int cur_line, cur_col, cur_pos;
} mt_file_t;

/**
 * @brief Checks if a file exists.
 * @return Whether the file exists.
 */
bool is_file(const char* path);

/**
 * @brief Checks if a directory exists.
 * @return Whether the directory exists.
 */
bool is_directory(const char* path);

/**
 * @brief Opens a file.
 * @param path The path to the file.
 * @return The file.
 */
mt_file_t* mt_file_init(const char* path);

/**
 * @brief Closes a file.
 * @param file The file to close.
 */
void mt_file_deinit(mt_file_t* file);

#endif // __MT_FILESYSTEM_H__
