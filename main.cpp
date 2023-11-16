#include "Libs/logs.h"
#include "list.h"
#include "list_logs.h"

int main()
{
    list_error err = LIST_NO_ERR;
    err = list_open_log_file();

    List list = {};

    add_log_file(stderr, LOG_LVL_TRACE);
    err = list_init(&list);
    err = list_push_back(&list, 2);
    err = list_push_back(&list, 2);
    err = list_delete_elem(&list, 0);
    err = list_linearize(&list);
    err = list_push_back(&list, 2);
    err = list_push_back(&list, 3);
    err = list_push_front(&list, 5);


    if (err != LIST_NO_ERR)
        LOG_ERROR("%s\n", list_get_error_msg(err));
    else
        LOG_INFO("%s\n", list_get_error_msg(err));

    list_close_log_file();

    return 0;
}
