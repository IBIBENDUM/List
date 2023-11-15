#ifndef LIST_H
#define LIST_H

#include <stddef.h>
#include <limits.h>

typedef int elem_t;
#define ELEM_FORMAT "%d"

const size_t LIST_MIN_CAPACITY = 8;
const elem_t LIST_POISON_VALUE = INT_MAX;
const int    LIST_PREV_POISON  = -2; // ???

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
    size_t  head;
    size_t  tail;

    size_t* next;
    size_t* prev;

    size_t free;
};

list_error list_init(List* list);

list_error list_verify(List* list);

list_error list_inert_(List* list);

list_error list_destruct(List* list);


#endif
