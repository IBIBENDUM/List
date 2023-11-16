#ifndef LIST_LOGS_H
#define LIST_LOGS_H

#include <time.h>

#include "list.h"

#define LOG_FOLDER_NAME         "log"
#define LOG_IMGS_FOLDER_NAME    "images"
#define LOG_DOTS_FOLDER_NAME    "dot"
#define LOG_GRAPH_NAME          "graph"
#define LOG_FILE_NAME           "list_log"

const int BG_COLOR            = 0xFFFFFF;
const int FILL_COLOR          = 0xFFE4C4;
const int OUTLINE_COLOR       = 0x800000;
const int NEXT_COLOR          = 0x00FF00;
const int PREV_COLOR          = 0x00FFFF;

const int MSG_ERROR_COLOR     = 0xFF0000;
const int MSG_DEFAULT_COLOR   = 0x00FFFF;
const int MSG_TIME_COLOR      = 0x888888;
const int MSG_LINE_INFO_COLOR = 0x000000;

struct log_call_line_info
{
    const char* file;
    const char* func;
    const int   line;
    struct tm*  time;
};

list_error generate_graph(const List* list);

list_error list_log_internal(const List* list, log_call_line_info* line_info);

#define list_log(LIST_PTR, ...)                       \
    do {                                              \
        time_t time_info = time(NULL);                \
        log_call_line_info call_line_info =           \
        {                                             \
            .file = __FILE__,                         \
            .func = __PRETTY_FUNCTION__,              \
            .line = __LINE__,                         \
            .time = localtime(&time_info)             \
        };                                            \
        list_log_internal(LIST_PTR, &call_line_info); \
    } while (0)

list_error list_open_log_file();

list_error list_close_log_file();

#endif
