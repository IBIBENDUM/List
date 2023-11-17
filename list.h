#ifndef LIST_H
#define LIST_H

#include <stddef.h>
#include <limits.h>

#include "list_config.h"

// BAH: make something
enum list_error
{
    #define DEF_LIST_ERR(NAME, ...) NAME,
    #include "list_errors.inc"
    // There is undef inside "list_errors.inc"
};

const char* const list_error_strings[]
{
    #define DEF_LIST_ERR(NAME, MESSAGE, ...) MESSAGE,
    #include "list_errors.inc"
    // There is undef inside "list_errors.inc"
};

// BAH: Ask about struct for arrays???
struct List
{
    elem_t* data;   ///< Elements values array
    int  size;      ///< Current elements amount
    int  capacity;  ///< Current allocated amount of memory for elements
                    ///  (expandable if necessary @see LIST_CAPACITY_MULTIPLIER)

    bool is_linear; ///< True if the index of the element in memory matches its position in the list

    int* next;      ///< Array of indexes on the next elements
    int* prev;      ///< Array of indexes on the previous elements

    int free;       ///< Index of first free element
};

/**
 * @brief  Get the error message string
 *
 * @return error message string pointer
 */
const char* list_get_error_msg(list_error err);

/**
 * @brief  Initialize list
 *
 * @param  list  - list pointer
 * @return
 *         LIST_NO_ERR        - no error occurred
 *         LIST_NULL_PTR      - list pointer is null
 *         LIST_MEM_ALLOC_ERR - memory allocation error during resize
 */
list_error list_init(List* list);

/**
 * @brief Check list for errors
 *
 * @param  list  - list pointer
 * @return
 *         LIST_NO_ERR                - no error occurred
 *         LIST_NULL_PTR              - list pointer is null
 *         LIST_NULL_DATA_ARR_PTR     - data pointer is null
 *         LIST_NULL_PREV_ARR_PTR     - prev pointer is null
 *         LIST_NULL_NEXT_ARR_PTR     - next pointer is null
 *         LIST_NEGATIVE_SIZE_ERR     - list size is below zero
 *         LIST_NEGATIVE_CAPACITY_ERR - list capacity is below zero
 *         LIST_NEGATIVE_FREE_IDX     - free index is below zero
 *         LIST_FREE_IDX_OUT_OF_RANGE - free index grater than capacity
 */
list_error list_verify(const List* list);

list_error list_realloc(List* list, int new_capacity);

/**
 * @brief  Destruct list
 *
 * @return
 *         LIST_NO_ERR        - no error occurred
 *         LIST_NULL_PTR      - list pointer is null
 */
list_error list_destruct(List* list);

/**
 * @brief  Insert an element after the element pointed to by the index
 *
 * @param  list  - list pointer
 * @param  value - element to be added to this list
 * @return errors returned by the verifier @see list_verify
 */
list_error list_insert_after(List* list, int pos, elem_t value);

/**
 * @brief  Insert an element before the element pointed to by the index
 *
 * @param  list  - list pointer
 * @param  value - element to be added to this list
 * @return errors returned by the verifier @see list_verify
 */
list_error list_insert_before(List* list, int idx, elem_t value);

/**
 * @brief  Push element to the end of the list
 *
 * @param  list  - list pointer
 * @param  value - element to be added to this list
 * @return errors returned by the verifier @see list_verify
 */
list_error list_push_back(List* list, elem_t value);

/**
 * @brief  Push element to the start of the list
 *
 * @note   If the list is full, reallocation will be
 *         performed with the specified coefficient @see LIST_CAPACITY_MULTIPLIER
 *
 * @param  list        - list pointer
 * @param  value       - element to be added to this list
 * @return errors returned by the verifier @see list_verify
 */
list_error list_push_front(List* list, elem_t value);


/**
 * @brief Remove element from list by index
 *
 * @note   if the ratio of capacity to the number of elements in the list
 *         is greater than the squared resize factor,
 *         it will be reduced by the resize factor times
 * @param  list - list pointer
 * @param  idx  - index of the element to be deleted
 * @return
 *         LIST_NO_ERR        - no error occurred
 *         LIST_FREE_IDX      - element is free (no deletion)
 *         LIST_MEM_ALLOC_ERR - memory allocation error during resize
 *         errors returned by the verifier @see list_verify
 */
list_error list_delete_elem(List* list, int idx);

/**
 * @brief Get element value from list by index
 *
 * @param list  - list pointer
 * @param idx   - index of the element to be returned
 * @param value - pointer where the value will be written
 * @return the errors returned are the same as the verifier @see list_verify
 */
list_error list_get_value(List* list, int idx, elem_t* value);

/**
 * @brief Get the number of the elements in the list
 *
 * @param list  - list pointer
 * @param size  - pointer where the size will be written
 * @return the errors returned are the same as the verifier @see list_verify
 */
list_error list_get_size(List* list, int* size);

/**
 * @brief Linearize list
 *
 * Rearrange elements in memory according
 * to their logical order in a list
 *
 * @param  list  - list pointer
 * @return
 *         LIST_NO_ERR        - no error occurred
 *         LIST_MEM_ALLOC_ERR - error allocating memory for new index arrays
 *         errors returned by the verifier @see list_verify
 */
list_error list_linearize(List* list);

#endif
