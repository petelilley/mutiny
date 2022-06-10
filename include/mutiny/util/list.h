#ifndef __MT_LIST_H__
#define __MT_LIST_H__

#include <stddef.h>

/**
 * @brief Represents a list of a specified type.
 *
 * @param type The type of each element in the list.
 */
#define list_t(type) \
  struct { size_t n; type* a; }

/**
 * @brief Initializes a list.
 */
#define l_init(l) \
  ((l).n = 0, (l).a = 0)

/**
 * @brief Frees a list from memory.
 */
#define l_deinit(l) \
  free((l).a)

/**
 * @brief Accesses a list at a specified index.
 */
#define l_at(l, i) \
  ((l).a[(i)])

/**
 * @brief Returns the number of elements in the list.
 */
#define l_size(l) \
  ((const size_t)(l).n)

/**
 * @brief Pushes a single element to the end of the list.
 */
#define l_push(l, type, e) \
  do {                                         \
    (l).a = (l).a ?                            \
      realloc((l).a, ++(l).n * sizeof(type)) : \
      malloc(++(l).n * sizeof(type));          \
    l_at(l, (l).n - 1) = e;                    \
  } while(0)


#endif // __MT_LIST_H__
