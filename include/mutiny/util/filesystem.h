#ifndef __MT_FILESYSTEM_H__
#define __MT_FILESYSTEM_H__

/**
 * @brief Represents a file (for reading purposes).
 */
typedef struct _mt_file {
  char* path;
  char* contents;
  char* ptr;
  long int cur_line, cur_col, cur_pos;
} mt_file_t;

#include <stdbool.h>

/**
 * @brief Checks if a given path is a file.
 *
 * @param path The path to a file.
 *
 * @return True if the path points to a file, false if not.
 */
bool is_file(const char* path);

/**
 * @brief Checks if a given path is a directory.
 *
 * @param path The path to a directory.
 *
 * @return True if the path points to a directory, false if not.
 */
bool is_directory(const char* path);

/**
 * @brief Opens a file and returns it's contents.
 *
 * @param path The path to a file.
 *
 * @return The contents of the file. NULL if empty or non-existent.
 *
 * @see mt_file_t
 */
mt_file_t* mt_file_init(const char* path);

/**
 * @brief De-initializes a file.
 *
 * @param file The file to de-initialize.
 */
void mt_file_deinit(mt_file_t* file);

#endif // __MT_FILESYSTEM_H__
