#ifndef LIST_CONFIG_H_
#define LIST_CONFIG_H_

// ~~~~~~~~~~~~~~~~~~~~~LIST_COMMON~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// BAH: Add different types
typedef int elem_t;       ///< element type
#define ELEM_FORMAT "%d"  ///< element format

const float  LIST_CAPACITY_MULTIPLIER = 2; ///< coefficient of increase (decrease) of list capacity

const size_t LIST_MIN_CAPACITY = 8;        ///< minimum list capacity (same after initialization)
const elem_t LIST_POISON_VALUE = INT_MAX;  ///< poison for element value
const int    LIST_PREV_POISON  = -2;       ///< poison for previous element index
const int    LIST_HEAD_INDEX   = -1;       ///< head element index

// ~~~~~~~~~~~~~~~~~~~~~~~LIST_LOGS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define LOG_FOLDER_NAME         "log"      ///< log folder name
#define LOG_IMGS_FOLDER_NAME    "images"   ///< images folder name (this folder is nested in the logs folder)
#define LOG_DOTS_FOLDER_NAME    "dot"      ///< images folder name (this folder is nested in the logs folder)
#define LOG_GRAPH_NAME          "graph"    ///< graph file (name without extension)
#define LOG_FILE_NAME           "list_log" ///< log file (name without extension)

const size_t IMAGE_WIDTH       = 1500; ///< Images width in pixels in html file

// Graphviz graph colors
const int BG_COLOR             = 0xFFFFFF;
const int FILL_COLOR           = 0xF5CDC1;
const int OUTLINE_COLOR        = 0xA0694D;
const int NEXT_COLOR           = 0x1695E2;
const int PREV_COLOR           = 0x29C56E;

// Html messages colors
const int MSG_ERROR_COLOR      = 0xFF0000;
const int MSG_TIME_COLOR       = 0x9DAABB;
const int MSG_LINE_INFO_COLOR  = 0x000000;

#endif
