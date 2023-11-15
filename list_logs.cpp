#include <stdio.h>

#include <assert.h>
#include <stdio.h>

#include "list_logs.h"

// #define log(file_ptr, (...) fprintf(file_ptr, (__VA_ARGS__))
#define COLOR_FORMAT "%06X"

static void write_node(List* list, FILE* file_ptr)
{
    assert(list);
    assert(file_ptr);

    for (int i = -1; i < list->capacity; i++)
    {
        fprintf(file_ptr, "%d [shape = Mrecord, style = filled, fillcolor = \"#" COLOR_FORMAT "\", color = \"#" COLOR_FORMAT "\", label = \"{PRV: %d|{IP: %d|DATA: %d} |NXT: %d}\"];\n",
        i, FILL_COLOR, OUTLINE_COLOR, *(list->prev + i), i, *(list->data + i), *(list->next + i));
    }
}

static void link_nodes_straight(List* list, FILE* file_ptr)
{
    assert(list);
    assert(file_ptr);

    for (int i = -1; i < list->capacity - 2; i++)
    {
        fprintf(file_ptr, "%d -> ", i);
    }
    fprintf(file_ptr, "%d ", list->capacity - 1);
    fprintf(file_ptr, "[color = \"#" COLOR_FORMAT "\", weight = 0]\n", BG_COLOR);
}

// BAH: CRINGEEEEEE
static void link_nodes_forward(List* list, FILE* file_ptr)
{
    assert(list);
    assert(file_ptr);

    for (int i = -1; i < list->capacity; i++)
    {
        fprintf(file_ptr, "%d -> %d ", i, list->next[i]);
        fprintf(file_ptr, "[color = \"#" COLOR_FORMAT "\", weight = 0]\n", NEXT_COLOR);
    }
    fprintf(file_ptr, "\n");
}

static void link_nodes_backward(List* list, FILE* file_ptr)
{
    for (int i = 0; i < (int)  list->capacity; i++)
    {
        if (list->prev[i] != LIST_PREV_POISON)
        {
            fprintf(file_ptr, "%d -> %d", i, list->prev[i]);
            fprintf(file_ptr, " [color = \"#" COLOR_FORMAT "\", weight = 0]\n", PREV_COLOR);
        }
    }
}

list_error generate_graph(List* list)
{
    // ???
    FILE* file_ptr = fopen("log.dot", "w");
    if (!file_ptr)
    {
        // ...
        return LIST_NULL_LOG_FILE_PTR_ERR;
    }
    fprintf(file_ptr,"digraph G{\n");
    fprintf(file_ptr,"rankdir = LR;\n");
    fprintf(file_ptr,"bgcolor = \"#" COLOR_FORMAT "\";\n", BG_COLOR);

    if (!list)
    {
        // BAH: write in log about it
    }

    write_node(list, file_ptr);
    link_nodes_straight(list, file_ptr);
    link_nodes_forward(list, file_ptr);
    link_nodes_backward(list, file_ptr);

    fprintf(file_ptr,"}\n");

    return LIST_NO_ERR;
}
