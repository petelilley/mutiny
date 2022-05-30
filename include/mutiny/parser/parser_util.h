#ifndef __MT_PARSER_UTIL_H__
#define __MT_PARSER_UTIL_H__

/**
 * @brief Compares the start of a string to another string.
 */
#define START(x, y) (strncmp((x), (y), strlen(y)) == 0)

/**
 * @brief Compares the end of a string to another string.
 */
#define END(x, y)   ((strlen(x) >= strlen(y)) && (strncmp((x) + strlen(x) - strlen(y), (y), strlen(y)) == 0))

#endif // __MT_PARSER_UTIL_H__
