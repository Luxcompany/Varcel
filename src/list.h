#ifndef VARCEL_LIST_H_
#define VARCEL_LIST_H_

#include <uint.h>

/** @brief opaque handle to list */
typedef struct List *List;

/** @brief dummy type to be casted to pointer of desired type*/
typedef void *Element;

/**
 * @brief creates a new list and returns a handle - must be paired with list_destroy
 * @param stride number of bytes between consecutive elements
 * @returns opaque handle to list
 */
List list_create_stride(unsigned int stride);

/**
 * @brief creates a new list and returns a handle - must be paired with list_destroy
 * @param type type to contain in list
 * @returns opaque handle to list
 */
#define list_create(type) list_create_stride(sizeof(type))

/**
 * @brief frees memory of a list created with list_create
 * @param list opaque handle of list to destroy
 */
void list_destroy(List list);

/**
 * @brief expands list if needed, and returns pointer to element
 * @param list list to append to -- cant be a copy by value
 * @returns element of list, or NULL if error
 */
Element list_append(List list);

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
 * @returns void ptr to new elemtn
 */
Element list_get_ptr(List list, uint index);

/**
 * @brief gets an element in list
 * @param list list to get element of
 * @param index index of element to get
 * @param type type of element to get
 * @returns element of type
 */
#define list_get(list, index, type) (*(type*)list_get_ptr(list, index))

/**
 * @brief returns a void pointer to the begining of list - to be casted to desired type pointer
 * @param list list to get ptr to
 * @returns void ptr to be casted
 */
void *list_ptr(List list);

#endif