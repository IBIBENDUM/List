#include <stdlib.h>

#include "list.h"

list_error list_init(List* list)
{
    if (!list)
        return LIST_NULL_PTR;

    // BAH: Check how do it better
    *list = {};
    elem_t* data_ptr = (elem_t*) calloc(LIST_MIN_CAPACITY + 1, sizeof(data_ptr[0]));
    if (!data_ptr)
        return LIST_MEM_ALLOC_ERR;

    size_t* free_ptr = (size_t*) calloc(LIST_MIN_CAPACITY + 1, sizeof(free_ptr[0]));
    if (!free_ptr)
    {
        free(data_ptr);
        return LIST_MEM_ALLOC_ERR;
    }

    size_t* next_ptr = (size_t*) calloc(LIST_MIN_CAPACITY + 1, sizeof(next_ptr[0]));
    if (!next_ptr)
    {
        free(data_ptr);
        free(free_ptr);
        return LIST_MEM_ALLOC_ERR;
    }

    size_t* prev_ptr = (size_t*) calloc(LIST_MIN_CAPACITY + 1, sizeof(prev_ptr[0]));
    if (!prev_ptr)
    {
        free(data_ptr);
        free(free_ptr);
        free(next_ptr);
        return LIST_MEM_ALLOC_ERR;
    }

    *list = (List) {
                    .data = data_ptr + 1,
                    .capacity = LIST_MIN_CAPACITY,
                    .next = next_ptr + 1,
                    .prev = prev_ptr + 1,
                    .free = 0
                   };

    for (size_t i = 0; i < LIST_MIN_CAPACITY; i++)
    {
        list->prev[i] = LIST_PREV_POISON;
        list->next[i] = i + 1;
    }
    list->next[LIST_MIN_CAPACITY - 1] = -1;

    list->prev[-1] = -1;
    list->next[-1] = -1;

    return LIST_NO_ERR;
}

list_error destruct_list(List* list)
{
    if (!list)
        return LIST_NULL_PTR;

    // ...
    return LIST_NO_ERR;
}
