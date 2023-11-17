#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "Libs/logs.h"
#include "Libs/utils.h"
#include "list.h"
#include "list_logs.h"

#define LIST_VERIFY_AND_RETURN_IF_ERR(LIST) \
    do                                      \
    {                                       \
        list_error err = list_verify(LIST); \
        if (err != LIST_NO_ERR)             \
            return err;                     \
    } while (0)

// BAH: ADD SWAP, REVERSE

// BAH: Maybe move to utils?
const char* list_get_error_msg(list_error err)
{
    return list_error_strings[err];
}

list_error list_verify(const List* list)
{
    if (list == NULL)
        return LIST_NULL_PTR_ERR;

    if (list->data == NULL)
        return LIST_NULL_DATA_ARR_PTR_ERR;

    if (list->prev == NULL)
        return LIST_NULL_PREV_ARR_PTR_ERR;

    if (list->next == NULL)
        return LIST_NULL_NEXT_ARR_PTR_ERR;

    if (list->size < 0)
        return LIST_NEGATIVE_SIZE_ERR;

    if (list->capacity < 0)
        return LIST_NEGATIVE_CAPACITY_ERR;

    if (list->free < 0)
        return LIST_NEGATIVE_FREE_IDX_ERR;

    if (list->free > list->capacity)
        return LIST_FREE_IDX_OUT_OF_RANGE_ERR;

    return LIST_NO_ERR;
}


/**
 * @brief Initialize head element
 *
 * Set the index of the previous element to the last element in the list
 * and the next element to the first element in the list
 * @param list  - list pointer
 */
static void list_init_head_elem(List* list)
{
    assert(list);

    list->next[LIST_HEAD_INDEX] = (list->size)? 0 : LIST_HEAD_INDEX;
    list->data[LIST_HEAD_INDEX] = 0;
    list->prev[LIST_HEAD_INDEX] = (list->size)? list->size - 1 : LIST_HEAD_INDEX;
}

/**
 * @brief Fill elements with poison values
 *
 * @param list      - list pointer
 * @param start_idx - the index of the start of the poison filling
 */
static void list_fill_with_poison(List* list, const int start_idx)
{
    assert(list);

    for (int idx = start_idx; idx < list->capacity; idx++)
    {
        list->next[idx] = idx + 1;
        list->prev[idx] = LIST_PREV_POISON;
        list->data[idx] = LIST_POISON_VALUE;
    }
    list->next[list->capacity - 1] = LIST_HEAD_INDEX;
}

list_error list_init(List* list)
{
    if (!list)
        return LIST_NULL_PTR_ERR;

    *list = (List){.is_linear = true};

    list_error err = list_realloc(list, LIST_MIN_CAPACITY);
    if (err != LIST_NO_ERR)
        return err;

    list_init_head_elem(list);

    list_log(list);

    return LIST_NO_ERR;
}

/**
 * @brief Get free element
 *
 * Get the first free element and change free index to the next free element
 * @param  list  - list pointer
 * @return index of first free element
 */
static int list_get_free_node_idx(List* list)
{
    assert(list);

    const int idx = list->free;
    list->free    = list->next[idx];

    return idx;
}

list_error list_realloc(List* list, int new_capacity)
{
    assert(list);

    elem_t* old_data_ptr = (list->capacity == 0)? NULL : list->data - 1;
    int*    old_prev_ptr = (list->capacity == 0)? NULL : list->prev - 1;
    int*    old_next_ptr = (list->capacity == 0)? NULL : list->next - 1;

    // BAH: or check every ptr separately and stop immediately if NULL???
    int*    prev_ptr = (int*)    realloc(old_prev_ptr, (new_capacity + 1) * sizeof(prev_ptr[0]));
    elem_t* data_ptr = (elem_t*) realloc(old_data_ptr, (new_capacity + 1) * sizeof(data_ptr[0]));
    int*    next_ptr = (int*)    realloc(old_next_ptr, (new_capacity + 1) * sizeof(next_ptr[0]));

    if (!prev_ptr || !data_ptr || !next_ptr)
    {
        free_and_null(list->data);
        free_and_null(list->prev);
        free_and_null(list->next);

        return LIST_MEM_ALLOC_ERR;
    }
    list->prev = prev_ptr + 1;
    list->data = data_ptr + 1;
    list->next = next_ptr + 1;

    int old_capacity = list->capacity;
    list->capacity = new_capacity;

    list->next[old_capacity - 1] = old_capacity;
    list_fill_with_poison(list, old_capacity);

    return LIST_NO_ERR;
}

/**
 * @brief Resize list down
 *
 * @param  list  - list pointer
 * @return errors returned by the verifier @see list_verify
 */
static list_error resize_down(List* list)
{
    assert(list);
    list_error err = LIST_NO_ERR;
    err = list_linearize(list);
    err = list_realloc(list, (int)((float)list->capacity / LIST_CAPACITY_MULTIPLIER));

    return err;
}

/**
 * @brief Resize list up
 *
 * @param  list  - list pointer
 * @return errors returned by the verifier @see list_verify
 */
static list_error resize_up(List* list)
{
    assert(list);
    list_error err = LIST_NO_ERR;
    err = list_realloc(list, (int)((float)list->capacity * LIST_CAPACITY_MULTIPLIER));

    return err;
}

list_error list_insert_after(List* list, const int idx, const elem_t value)
{
    LIST_VERIFY_AND_RETURN_IF_ERR(list);

    if (idx < LIST_HEAD_INDEX)
        return LIST_IDX_OUT_OF_RANGE_ERR;

    list_error err = LIST_NO_ERR;
    if (list->size + 1 >= list->capacity)
        resize_up(list);

    if (err != LIST_NO_ERR)
        return err;

    const int free_idx     = list_get_free_node_idx(list);
    int*      next_free    = &list->next[idx];

    list->prev[free_idx]   = idx;
    list->data[free_idx]   = value;
    list->next[free_idx]   = *next_free;

    list->prev[*next_free] = free_idx;
    *next_free = free_idx;

    list->size++;

    if (list->next[LIST_HEAD_INDEX] != 0)
        list->is_linear = false;

    list_log(list);

    return LIST_NO_ERR;
}

list_error list_insert_before(List* list, const int idx, const elem_t value)
{
    return list_insert_after(list, list->prev[idx], value);
}

list_error list_push_front(List* list, const elem_t value)
{
    return list_insert_after(list, LIST_HEAD_INDEX, value);
}

list_error list_push_back(List* list, const elem_t value)
{
    return list_insert_before(list, LIST_HEAD_INDEX, value);
}

/**
 * @brief  Find out if an element is free
 *
 * @param  list - list pointer
 * @return errors returned by the verifier @see list_verify
 */
static bool is_free_elem(List* list, const int idx)
{
    assert(list);
    if (list->prev[idx] == LIST_PREV_POISON)
        return true;

    return false;
}

/**
 * @brief  Make element free
 *
 * @param  list - list pointer
 * @return errors returned by the verifier @see list_verify
 */
static list_error list_free_elem(List* list, const int idx)
{
    LIST_VERIFY_AND_RETURN_IF_ERR(list);

    list->prev[idx] = LIST_PREV_POISON;
    list->data[idx] = LIST_POISON_VALUE;
    list->next[idx] = list->free;
    list->free      = idx;

    return LIST_NO_ERR;
}

list_error list_delete_elem(List* list, const int idx)
{
    LIST_VERIFY_AND_RETURN_IF_ERR(list);

    if (is_free_elem(list, idx))
        return LIST_FREE_IDX;

    list_error err = LIST_NO_ERR;
    if ((int) LIST_MIN_CAPACITY < list->size && list->size * (int)(LIST_CAPACITY_MULTIPLIER * LIST_CAPACITY_MULTIPLIER) < list->capacity)
        err = resize_down(list);

    if (err != LIST_NO_ERR)
        return err;

    list->is_linear = false;

    const int prev_idx   = list->prev[idx];
    const int next_idx   = list->next[idx];
    list->next[prev_idx] = list->next[idx];
    list->prev[next_idx] = list->prev[idx];

    list_free_elem(list, idx);

    list->size--;

    list_log(list);

    return LIST_NO_ERR;
}

list_error list_get_value(List* list, const int idx, elem_t* value)
{
    LIST_VERIFY_AND_RETURN_IF_ERR(list);

    if (is_free_elem(list, idx))
        return LIST_FREE_IDX;

    *value = list->data[idx];

    return LIST_NO_ERR;
}

list_error list_get_size(List* list, int* size)
{
    LIST_VERIFY_AND_RETURN_IF_ERR(list);

    *size = list->size;
    return LIST_NO_ERR;
}

list_error list_destruct(List* list)
{
    if (!list)
        return LIST_NULL_PTR_ERR;

    // BAH: is free NULL ok?
    free_and_null(list->data - 1);
    free_and_null(list->prev - 1);
    free_and_null(list->next - 1);
    *list = {};

    return LIST_NO_ERR;
}

list_error list_linearize(List* list)
{
    LIST_VERIFY_AND_RETURN_IF_ERR(list);

    if (list->is_linear)
        return LIST_NO_ERR;

    // BAH: Reallocation or swap elements???
    // BAH: How to remove copypaste???
    int*    new_prev_ptr = (int*)    calloc(list->capacity + 1, sizeof(new_prev_ptr[0]));
    elem_t* new_data_ptr = (elem_t*) calloc(list->capacity + 1, sizeof(new_data_ptr[0]));
    int*    new_next_ptr = (int*)    calloc(list->capacity + 1, sizeof(new_next_ptr[0]));

    if (!new_prev_ptr || !new_data_ptr || !new_next_ptr)
    {
        free_and_null(new_prev_ptr);
        free_and_null(new_data_ptr);
        free_and_null(new_next_ptr);

        return LIST_MEM_ALLOC_ERR;
    }
    new_prev_ptr++;
    new_data_ptr++;
    new_next_ptr++;

    int physical_idx = list->next[LIST_HEAD_INDEX];
    for (int idx = 0; idx < list->size; idx++)
    {
        new_prev_ptr[idx] = idx - 1;
        new_data_ptr[idx] = list->data[physical_idx];
        new_next_ptr[idx] = idx + 1;
        physical_idx      = list->next[physical_idx];
    }
    free_and_null(list->data - 1);
    free_and_null(list->prev - 1);
    free_and_null(list->next - 1);

    list->data = new_data_ptr;
    list->prev = new_prev_ptr;
    list->next = new_next_ptr;

    list_fill_with_poison(list, list->size);
    list_init_head_elem(list);

    list->next[list->size - 1] = LIST_HEAD_INDEX;
    list->free = list->size;
    list->is_linear = true;

    list_log(list);

    return LIST_NO_ERR;
}
