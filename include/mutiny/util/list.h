#ifndef __MT_LIST_H__
#define __MT_LIST_H__

#include <stddef.h>

/**
 * @brief Represents a list.
 * @param type The data type of the list.
 */
#define list_t(type) \
  struct { size_t n; type* a; size_t s; }

/**
 * @brief Initializes a list.
 * @param l The list to initialize.
 * @param type The data type of the list.
 */
#define l_init(l, type) \
  ((l).n = 0, (l).a = 0, (l).s = sizeof(type))

/**
 * @brief Destroys a list.
 * @param l The list to destroy.
 */
#define l_deinit(l) \
  do {              \
    if ((l).a) {    \
      free((l).a);  \
    }               \
  } while (0)

/**
 * @brief Accesses the nth element of a list.
 * @param l The list.
 * @param n The index of the element.
 */
#define l_at(l, n) \
  ((l).a[(n)])

/**
 * @brief The number of elements in a list.
 * @param l The list to get the number of elements from.
 */
#define l_size(l) \
  ((const size_t)(l).n)

/**
 * @brief Adds an element to the end of a list.
 * @param l The list to add to.
 * @param e The element to add.
 */
#define l_push(l, e) \
  do {                                         \
    (l).a = (l).a ?                            \
      realloc((l).a, ++(l).n * (l).s) : \
      malloc(++(l).n * (l).s);          \
    l_at(l, (l).n - 1) = e;                    \
  } while (0)

#endif // __MT_LIST_H__
