#ifndef LIST_LOGS_H
#define LIST_LOGS_H

#include <time.h>

#include "list.h"
#include "list_config.h"

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

// Information about line from which log is called
struct log_call_line_info
{
    const char* file;
    const char* func;
    const int   line;
    struct tm*  time;
};

/**
 * @brief  Log list with graphviz implementation
 *
 * @param  list - list pointer
 * @param  line_info
 * @return
 *         LIST_NO_ERR    - no error occurred
 *         LIST_NULL_PTR  - list pointer is NULL
 */
list_error list_log_internal(const List* list, log_call_line_info* line_info);

/**
 * @brief  Open log file
 *
 * @return
 *         LIST_NO_ERR         - no error occurred
 *         LIST_FILE_OPEN_ERR  - file opening error
 */
list_error list_open_log_file();

/**
 * @brief  Close log file
 *
 * @return
 *         LIST_NO_ERR         - no error occurred
 *         LIST_FILE_CLOSE_ERR - file closing error
 */
list_error list_close_log_file();

#endif
