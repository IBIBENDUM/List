#ifndef LIST_H
#define LIST_H

#include <stddef.h>
#include <limits.h>

typedef int elem_t;
#define ELEM_FORMAT "%d"

const size_t LIST_MIN_CAPACITY = 8;
const elem_t LIST_POISON_VALUE = INT_MAX;
const int    LIST_PREV_POISON  = -2; // ???
const int    LIST_HEAD_INDEX   = -1;

// BAH: make something
enum list_error
{
    #define DEF_LIST_ERR(NAME, ...) NAME,
    #include "list_errors.inc"
    // There is undef inside "list_errors.inc"
};

// const char* list_error_strings[]
// {
//     #define DEF_LIST_ERR(NAME, MESSAGE, ...) MESSAGE,
//     #include "list_errors.inc"
//     // There is undef inside "list_errors.inc"
// };

struct List
{
    elem_t* data;
    int  size;
    int  capacity;

    bool is_sorted;

    int* next;
    int* prev;

    int free;
};

list_error list_init(List* list);

list_error list_verify(const List* list);

list_error list_realloc(List* list, int new_capacity);

list_error list_destruct(List* list);

list_error list_insert_after(List* list, int pos, elem_t value);

list_error list_insert_before(List* list, int idx, elem_t value);

list_error list_push_back(List* list, elem_t value);

list_error list_push_front(List* list, elem_t value);

list_error list_delete_elem(List* list, int idx);

list_error list_get_value(List* list, int idx, elem_t* value);

list_error list_get_size(List* list, int* size);

#endif
