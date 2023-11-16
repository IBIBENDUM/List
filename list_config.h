#ifndef LIST_CONFIG_H_
#define LIST_CONFIG_H_

// BAH: Add different types
typedef int elem_t;       ///< element type
#define ELEM_FORMAT "%d"  ///< element format

const float  LIST_CAPACITY_MULTIPLIER = 2; ///< coefficient of increase (decrease) of list capacity

const size_t LIST_MIN_CAPACITY = 8;        ///< minimum list capacity (same after initialization)
const elem_t LIST_POISON_VALUE = INT_MAX;  ///< poison for element value
const int    LIST_PREV_POISON  = -2;       ///< poison for previous element index
const int    LIST_HEAD_INDEX   = -1;       ///< head element index

#define LOG_FOLDER_NAME         "log"      ///< log folder name
#define LOG_IMGS_FOLDER_NAME    "images"   ///< images folder name (this folder is nested in the logs folder)
#define LOG_DOTS_FOLDER_NAME    "dot"      ///< images folder name (this folder is nested in the logs folder)
#define LOG_GRAPH_NAME          "graph"    ///< graph file (name without extension)
#define LOG_FILE_NAME           "list_log" ///< log file (name without extension)

// Graphviz graph colors
const int BG_COLOR            = 0xFFFFFF;
const int FILL_COLOR          = 0xFFE4C4;
const int OUTLINE_COLOR       = 0x800000;
const int NEXT_COLOR          = 0x00FF00;
const int PREV_COLOR          = 0x00FFFF;

// Html messages colors
const int MSG_ERROR_COLOR     = 0xFF0000;
const int MSG_DEFAULT_COLOR   = 0x00FFFF;
const int MSG_TIME_COLOR      = 0x888888;
const int MSG_LINE_INFO_COLOR = 0x000000;

#endif
