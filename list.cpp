#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "Libs/logs.h"
#include "list.h"

#define LIST_VERIFY_AND_RETURN_IF_ERR(LIST) \
    do                                      \
    {                                       \
        list_error err = list_verify(LIST); \
        if (err != LIST_NO_ERR)             \
            return err;                     \
    } while (0)

// BAH: make bitmask
list_error list_verify(const List* list)
{
    if (list == NULL)
        return LIST_NULL_PTR;

    if (list->data == NULL)
        return LIST_NULL_DATA_ARR_PTR;

    if (list->prev == NULL)
        return LIST_NULL_PREV_ARR_PTR;

    if (list->next == NULL)
        return LIST_NULL_NEXT_ARR_PTR;

    if (list->free < 0)
        return LIST_NEGATIVE_FREE_IDX;

    if (list->free > list->capacity)
        return LIST_FREE_IDX_OUT_OF_RANGE;

    return LIST_NO_ERR;
}

list_error list_init(List* list)
{
    if (!list)
        return LIST_NULL_PTR;

    *list = (List)  {
                    .data = (elem_t*) 0 + 1, // BAH: ????
                    .capacity = 0,
                    .next = (int*) 0 + 1,
                    .prev = (int*) 0 + 1,
                    };

    list_realloc(list, LIST_MIN_CAPACITY);

    // Reserved element initialize
    list->prev[LIST_HEAD_INDEX] = LIST_HEAD_INDEX;
    list->data[LIST_HEAD_INDEX] =  0;
    list->next[LIST_HEAD_INDEX] = LIST_HEAD_INDEX;

    return LIST_NO_ERR;
}

static int list_get_free_node_idx(List* list)
{
    LIST_VERIFY_AND_RETURN_IF_ERR(list);

    const int idx = list->free;
    list->free = list->next[idx];

    return idx;
}

list_error list_realloc(List* list, int new_capacity)
{
    assert(list);
    // BAH: if new_capacity < old capacity?

    // BAH: or check every ptr separately and stop immediately if NULL???
    int*    prev_ptr = (int*)    realloc(list->prev - 1, (new_capacity + 1) * sizeof(prev_ptr[0]));
    elem_t* data_ptr = (elem_t*) realloc(list->data - 1, (new_capacity + 1) * sizeof(data_ptr[0]));
    int*    next_ptr = (int*)    realloc(list->next - 1, (new_capacity + 1) * sizeof(next_ptr[0]));

    if (!list->data || !list->prev || !list->next)
    {
        free(list->data);
        free(list->prev);
        free(list->next);

        return LIST_MEM_ALLOC_ERR;
    }

    list->prev = prev_ptr + 1;
    list->data = data_ptr + 1;
    list->next = next_ptr + 1;

    for (int idx = list->capacity - 1; idx < new_capacity; idx++)
    {
        list->next[idx] = idx + 1;
        list->prev[idx] = LIST_PREV_POISON;
        list->data[idx] = LIST_POISON_VALUE;
    }
    list->next[new_capacity - 1] = LIST_HEAD_INDEX;

    list->capacity = new_capacity;

    return LIST_NO_ERR;
}

list_error list_insert_after(List* list, const int idx, const elem_t value)
{
    LIST_VERIFY_AND_RETURN_IF_ERR(list);
    // BAH: Check for negative position

    const int free_idx = list_get_free_node_idx(list);
    int* next_free = &list->next[idx];

    list->prev[free_idx]  = idx;
    list->data[free_idx]  = value;
    list->next[free_idx]  = *next_free;

    list->prev[*next_free] = free_idx;
    *next_free = free_idx;

    list->size++;

    return LIST_NO_ERR;
}

list_error list_insert_before(List* list, const int idx, const elem_t value)
{
    // add checks
    return list_insert_after(list, list->prev[idx], value);
}

list_error list_push_front(List* list, const elem_t value)
{
    // add checks
    return list_insert_after(list, LIST_HEAD_INDEX, value);
}

list_error list_push_back(List* list, const elem_t value)
{
    // add checks
    return list_insert_before(list, LIST_HEAD_INDEX, value);
}

static bool is_free_elem(List* list, const int idx)
{
    LIST_VERIFY_AND_RETURN_IF_ERR(list);
    if (list->prev[idx] == LIST_PREV_POISON)
        return true;
    return false;
}

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

    // BAH: Check is node free already?
    const int prev_idx = list->prev[idx];
    const int next_idx = list->next[idx];
    list->next[prev_idx] = list->next[idx];
    list->prev[next_idx] = list->prev[idx];

    list_free_elem(list, idx);

    list->size--;

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
        return LIST_NULL_PTR;

    // BAH: is free NULL ok?
    free(list->data - 1);
    free(list->prev - 1);
    free(list->next - 1);
    *list = {};

    return LIST_NO_ERR;
}

list_error list_linearize(List* list)
{
    LIST_VERIFY_AND_RETURN_IF_ERR(list);

    // BAH: check for empty list or something like that
    int*    new_prev_ptr = (int*)    calloc(list->capacity + 1, sizeof(new_prev_ptr[0]));
    elem_t* new_data_ptr = (elem_t*) calloc(list->capacity + 1, sizeof(new_data_ptr[0]));
    int*    new_next_ptr = (int*)    calloc(list->capacity + 1, sizeof(new_next_ptr[0]));

    if (!new_prev_ptr || !new_data_ptr || !new_next_ptr)
        return LIST_MEM_ALLOC_ERR;

    int physical_idx = list->next[LIST_HEAD_INDEX];
    for (int idx = 0; idx != list->prev[LIST_HEAD_INDEX]; idx++)
    {
        new_prev_ptr[idx] = idx - 1;
        new_data_ptr[idx] = list->data[physical_idx];
        new_next_ptr[idx] = idx + 1;
        physical_idx    = list->next[idx];
    }

    new_next_ptr[LIST_HEAD_INDEX] = list->size;
    new_next_ptr[list->size] = LIST_HEAD_INDEX;

    list->data = new_data_ptr;
    list->prev = new_prev_ptr;
    list->next = new_next_ptr;

    return LIST_NO_ERR;
}
