#ifndef LIST_LOGS_H
#define LIST_LOGS_H

#include "list.h"

const int BG_COLOR      = 0xFFFFFF;
const int FILL_COLOR    = 0xFFE4C4;
const int OUTLINE_COLOR = 0x800000;
const int NEXT_COLOR    = 0x00FF00;
const int PREV_COLOR    = 0x00FFFF;

#define LOG_FOLDER_NAME      "log"
#define LOG_IMGS_FOLDER_NAME "images"
#define LOG_DOTS_FOLDER_NAME "dot"
#define LOG_GRAPH_NAME       "graph"
#define LOG_FILE_NAME        "list_log"

list_error generate_graph(const List* list);

list_error list_log(const List* list);

#endif
