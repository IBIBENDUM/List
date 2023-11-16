#include <stdio.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Libs/logs.h"
#include "Libs/utils.h"
#include "Libs/time_utils.h"
#include "list_logs.h"

#define print(...)                 fprintf(file_ptr, __VA_ARGS__)
#define log(...)                   fprintf(log_file_ptr, __VA_ARGS__)
#define log_color(COLOR, STR, ...) fprintf(log_file_ptr, "<font color = #%06X> " STR "</font>", COLOR, ##__VA_ARGS__)

#define COLOR_FORMAT "%06X"
#define NODE_SETTINGS_FORMAT "[color = \"#" COLOR_FORMAT "\", weight = %d]\n"

const size_t IMAGE_WIDTH = 1500;

const size_t GRAPH_MAX_PATH_LEN = 128;
FILE* log_file_ptr = NULL;
size_t graph_idx = 1;

static void write_node(const List* list, FILE* file_ptr)
{
    assert(list);
    assert(file_ptr);

    for (int i = -1; i < list->capacity; i++)
    {
        print("%d [shape = Mrecord, style = filled, fillcolor = \"#" COLOR_FORMAT "\", color = \"#" COLOR_FORMAT "\", label = \"{PRV: %d|{IP: %d|DATA: %d} |NXT: %d}\"];\n",
        i, FILL_COLOR, OUTLINE_COLOR, list->prev[i], i, list->data[i], list->next[i]);
    }
}

static void link_nodes_straight(const List* list, FILE* file_ptr)
{
    assert(list);
    assert(file_ptr);

    for (int i = -1; i < list->capacity - 1; i++)
    {
        print("%d -> ", i);
    }
    print("%d ", list->capacity - 1);
    print(NODE_SETTINGS_FORMAT, BG_COLOR, 1000);
}

static void link_nodes_forward(const List* list, FILE* file_ptr)
{
    assert(list);
    assert(file_ptr);

    for (int i = -1; i < list->capacity; i++)
    {
        print("%d -> %d ", i, list->next[i]);
        print(NODE_SETTINGS_FORMAT, NEXT_COLOR, 0);
    }
    print("\n");
}

static void link_nodes_backward(const List* list, FILE* file_ptr)
{
    assert(list);
    assert(file_ptr);

    for (int i = 0; i < (int)  list->capacity; i++)
    {
        if (list->prev[i] != LIST_PREV_POISON)
        {
            print("%d -> %d ", i, list->prev[i]);
            print(NODE_SETTINGS_FORMAT, PREV_COLOR, 0);
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
    print("digraph G{\n");
    print("rankdir = LR;\n");
    print("graph [splines=ortho];\n");
    print("bgcolor = \"#" COLOR_FORMAT "\";\n", BG_COLOR);
}

static char* get_file_graph_name(const char* extension)
{
    static char graph_name[GRAPH_MAX_PATH_LEN] = "";
    sprintf(graph_name, LOG_GRAPH_NAME "_%02llu.%s", graph_idx, extension);
    return graph_name;
}

static char* get_file_graph_path(const char* directory, const char* extension)
{
    assert(directory);
    assert(extension);

    static char graph_path[GRAPH_MAX_PATH_LEN] = "";
    snprintf(graph_path, GRAPH_MAX_PATH_LEN, "%s%s", directory, get_file_graph_name(extension));
    return graph_path;
}

static list_error open_graph_file(FILE** file_ptr)
{
    const char* dot_file_path = get_file_graph_path(LOG_FOLDER_NAME "/" LOG_DOTS_FOLDER_NAME "/", "dot");
    *file_ptr = fopen(dot_file_path, "w");
    if (!*file_ptr)
        return LIST_FILE_OPEN_ERR;

    return LIST_NO_ERR;
}

static void clear_log_files()
{
    #if _WIN32
        system("del /s /q " LOG_FOLDER_NAME);
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

    print("}\n");

    if (fclose(file_ptr))
        return LIST_FILE_CLOSE_ERR;

    return LIST_NO_ERR;
}

list_error list_open_log_file()
{
    clear_log_files();

    FILE* file_ptr = fopen(LOG_FOLDER_NAME "/" LOG_FILE_NAME ".html", "w");
    if (!file_ptr)
        return LIST_FILE_OPEN_ERR;

    log_file_ptr = file_ptr;

    log("<pre>\n");

    return LIST_NO_ERR;
}

list_error list_close_log_file()
{
    if (fclose(log_file_ptr))
        return LIST_FILE_CLOSE_ERR;

    return LIST_NO_ERR;
}

static void compile_graph()
{
    char terminal_command[GRAPH_MAX_PATH_LEN] = "";

    const char* dot_path = get_file_graph_path(LOG_FOLDER_NAME "/" LOG_DOTS_FOLDER_NAME "/", "dot");
    size_t symbols_amount = sprintf(terminal_command, "dot -Tpng %s ", dot_path);

    const char* png_path = get_file_graph_path(LOG_FOLDER_NAME "/" LOG_IMGS_FOLDER_NAME "/", "png");
    sprintf(terminal_command + symbols_amount, "-o %s", png_path);

    system(terminal_command);
}

static void add_image_to_log()
{
    const char* png_path = get_file_graph_path(LOG_IMGS_FOLDER_NAME "/", "png");
    log("\n<img src = \"%s\", width= \"%llu\">\n", png_path, IMAGE_WIDTH);
}

static list_error write_graph(const List* list)
{
    assert(list);

    list_error err = generate_graph(list);
    if (err != LIST_NO_ERR)
        return err;

    compile_graph();
    add_image_to_log();

    graph_idx++;

    return LIST_NO_ERR;
}

static void write_event_header(log_call_line_info* line_info)
{
    assert(line_info);

    const char* time_str = cast_time_to_str(line_info->time);
    log_color(MSG_TIME_COLOR, "[%s] ", time_str);
    log_color(MSG_LINE_INFO_COLOR, "%s:%s:%d: ", line_info->file, line_info->func, line_info->line);
}

list_error list_log_internal(const List* list, log_call_line_info* line_info)
{
    if (!list)
    {
        log_color(MSG_ERROR_COLOR, "ERROR: NULL list pointer\n");
        return LIST_NULL_PTR;
    }

    write_event_header(line_info);
    write_graph(list);

    log("\n\n");

    return LIST_NO_ERR;
}
