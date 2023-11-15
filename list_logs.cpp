#include <stdio.h>

#include <assert.h>
#include <stdio.h>

#include "Libs/logs.h"
#include "list_logs.h"

#define log(...) fprintf(file_ptr, __VA_ARGS__)
#define COLOR_FORMAT "%06X"

static void write_node(const List* list, FILE* file_ptr)
{
    assert(list);
    assert(file_ptr);

    for (int i = -1; i < list->capacity; i++)
    {
        log("%d [shape = Mrecord, style = filled, fillcolor = \"#" COLOR_FORMAT "\", color = \"#" COLOR_FORMAT "\", label = \"{PRV: %d|{IP: %d|DATA: %d} |NXT: %d}\"];\n",
        i, FILL_COLOR, OUTLINE_COLOR, *(list->prev + i), i, *(list->data + i), *(list->next + i));
    }
}

static void link_nodes_straight(const List* list, FILE* file_ptr)
{
    assert(list);
    assert(file_ptr);

    for (int i = -1; i < list->capacity - 1; i++)
    {
        log("%d -> ", i);
    }
    log("%d ", list->capacity - 1);
    log("[color = \"#" COLOR_FORMAT "\", weight = 1000]\n", BG_COLOR);
}

// BAH: CRINGEEEEEE
static void link_nodes_forward(const List* list, FILE* file_ptr)
{
    assert(list);
    assert(file_ptr);

    for (int i = -1; i < list->capacity; i++)
    {
        log("%d -> %d ", i, list->next[i]);
        log("[color = \"#" COLOR_FORMAT "\", weight = 0]\n", NEXT_COLOR);
    }
    log("\n");
}

static void link_nodes_backward(const List* list, FILE* file_ptr)
{
    for (int i = 0; i < (int)  list->capacity; i++)
    {
        if (list->prev[i] != LIST_PREV_POISON)
        {
            log("%d -> %d", i, list->prev[i]);
            log(" [color = \"#" COLOR_FORMAT "\", weight = 0]\n", PREV_COLOR);
        }
    }
}

list_error generate_graph(const List* list)
{
    // ???
    FILE* file_ptr = fopen("log.dot", "w");
    if (!file_ptr)
    {
        // ...
        return LIST_NULL_LOG_FILE_PTR_ERR;
    }
    log("digraph G{\n");
    log("rankdir = LR;\n");
    log("graph [splines=ortho];");
    log("bgcolor = \"#" COLOR_FORMAT "\";\n", BG_COLOR);

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

list_error list_log(const List* list)
{

}
