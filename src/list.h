#ifndef VARCEL_LIST_H_
#define VARCEL_LIST_H_

/** @brief opaque handle to list */
typedef struct List *List;

/** @brief dummy type to be casted to pointer of desired type*/
typedef void *Element;

/**
 * @brief creates a new list and returns a handle - must be paired with list_destroy
 * @param stride number of bytes between consecutive elements
 * @returns opaque handle to list
 */
List list_create(unsigned int stride);
/**
 * @brief frees memory of a list created with list_create
 * @param list opaque handle of list to destroy
 */
void list_destroy(List list);

/**
 * @brief expands list if needed, and returns pointer to element
 * @param list pointer to list to append to
 * @returns element of list, or NULL if error
 */
Element list_append(List *list);

/**
 * @brief gets size of list
 * @param list list to get size of
 * @returns unsigned int num of elements in list
 */
unsigned int list_size(List list);

/**
 * @brief gets possible size a list can be before expanding
 * @param list list to get possible size of
 * @returns unsigned int num of possible elements in list
 */
unsigned int list_possible_size(List list);

/**
 * @brief gets stride between consecutive list elements
 * @param list list to get stride of
 * @returns unsigned int num of stride
 */
unsigned int list_stride(List list);

/**
 * @brief gets an element in list
 * @param list list to get element of
 * @param index index of element to get
 * @returns element of list, or NULL if error
 */
Element list_get(List list, unsigned int index);

#endif