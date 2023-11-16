#include "Libs/logs.h"
#include "list.h"
#include "list_logs.h"

int main()
{
    list_open_log_file();

    List list = {};
    add_log_file(stderr, LOG_LVL_TRACE);

    FILE* test = fopen("log_test.log", "w");
    add_log_file(test, LOG_LVL_TRACE);

    list_init(&list);
    // list_realloc(&list, 12);

    list_push_front(&list, 1);
    list_push_back(&list, 2);
//     list_push_back(&list, 3);
//     list_push_back(&list, 4);
//

    elem_t value = 0;
    LOG_DEBUG("value = %d", value);
    list_delete_elem(&list, 0);

    list_log(NULL);
    list_log(&list);
    list_log(&list);
    list_log(&list);
    list_log(&list);
    generate_graph(&list);

    list_close_log_file();

    return 0;
}
