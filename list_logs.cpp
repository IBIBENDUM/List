#include <stdio.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Libs/logs.h"
#include "Libs/utils.h"
#include "list_logs.h"

#define log(...) fprintf(file_ptr, __VA_ARGS__)

#define COLOR_FORMAT "%06X"
#define NODE_SETTINGS_FORMAT "[color = \"#" COLOR_FORMAT "\", weight = %d]\n"

const size_t GRAPH_MAX_NAME_LEN = 64;
FILE* log_file_ptr = NULL;

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
    log(NODE_SETTINGS_FORMAT, BG_COLOR, 1000);
}

// BAH: CRINGEEEEEE
static void link_nodes_forward(const List* list, FILE* file_ptr)
{
    assert(list);
    assert(file_ptr);

    for (int i = -1; i < list->capacity; i++)
    {
        log("%d -> %d ", i, list->next[i]);
        log(NODE_SETTINGS_FORMAT, NEXT_COLOR, 0);
    }
    log("\n");
}

static void link_nodes_backward(const List* list, FILE* file_ptr)
{
    assert(list);
    assert(file_ptr);

    for (int i = 0; i < (int)  list->capacity; i++)
    {
        if (list->prev[i] != LIST_PREV_POISON)
        {
            log("%d -> %d ", i, list->prev[i]);
            log(NODE_SETTINGS_FORMAT, PREV_COLOR, 0);
        }
    }
}

static list_error create_log_folders()
{
    if (!create_folder(LOG_FOLDER_NAME))
        return LIST_COULDNT_CREATE_LOG_FOLDER;

    if (!create_folder(LOG_FOLDER_NAME "/" LOG_IMGS_FOLDER_NAME))
        return LIST_COULDNT_CREATE_LOG_FOLDER;

    if (!create_folder(LOG_FOLDER_NAME "/" LOG_DOTS_FOLDER_NAME))
        return LIST_COULDNT_CREATE_LOG_FOLDER;

    return LIST_NO_ERR;
}

static void write_graph_header(FILE* file_ptr)
{
    log("digraph G{\n");
    log("rankdir = LR;\n");
    log("graph [splines=ortho];");
    log("bgcolor = \"#" COLOR_FORMAT "\";\n", BG_COLOR);
}

static list_error open_graph_file(FILE** file_ptr)
{
    static size_t graph_idx = 1;
    char graph_name[GRAPH_MAX_NAME_LEN] = "";
    sprintf(graph_name, LOG_FOLDER_NAME "/" LOG_DOTS_FOLDER_NAME "/" LOG_GRAPH_NAME "_%02lld.dot", graph_idx);

    *file_ptr = fopen(graph_name, "w");
    if (!*file_ptr)
        return LIST_FILE_OPEN_ERR;

    return LIST_NO_ERR;
}

static void clear_log_files()
{
    #if _WIN32
        system("del /s /q" LOG_FOLDER_NAME);
    #else
        system("rm -r" LOG_FOLDER_NAME);
    #endif
}

list_error generate_graph(const List* list)
{
    assert(list);
    list_error err = LIST_NO_ERR;

    err = create_log_folders();
    if (err != LIST_NO_ERR)
        return err;

    FILE* file_ptr = NULL;
    err = open_graph_file(&file_ptr);
    if (err != LIST_NO_ERR)
        return err;

    write_graph_header(file_ptr);

    write_node(list, file_ptr);
    link_nodes_straight(list, file_ptr);
    link_nodes_forward(list, file_ptr);
    link_nodes_backward(list, file_ptr);

    log("}\n");

    if (fclose(file_ptr))
        return LIST_FILE_CLOSE_ERR;

    return LIST_NO_ERR;
}

list_error list_open_log_file()
{
    clear_log_files();

    FILE* log_file_ptr = fopen(LOG_FILE_NAME ".html", "w");
    if (!log_file_ptr)
        return LIST_FILE_OPEN_ERR;

    return LIST_NO_ERR;
}

list_error list_close_log_file()
{
    if (fclose(log_file_ptr))
        return LIST_FILE_CLOSE_ERR;

    return LIST_NO_ERR;
}

list_error list_log(const List* list)
{
    if (!list)
    {

    }

    return LIST_NO_ERR;
}
