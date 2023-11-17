#include "Libs/logs.h"
#include "Libs/console_args.h"
#include "list.h"
#include "list_logs.h"

int main(const int argc, char* const* argv)
{
    Args_values values = {};
    if (!handle_cmd_args(argc, argv, "i:o:l:m:h", &values))
        return 1;

    list_error err = LIST_NO_ERR;
    list_open_log_file();

    List list = {};
    list_init(&list);
    list_push_back(&list, 2);
    list_push_back(&list, 2);
    list_delete_elem(&list, 0);
    list_linearize(&list);
    list_push_back(&list, 2);
    list_push_back(&list, 3);
    err = list_push_front(&list, 5);

    if (err != LIST_NO_ERR)
        LOG_ERROR("%s\n", list_get_error_msg(err));
    else
        LOG_INFO("%s\n", list_get_error_msg(err));

    list_close_log_file();

    return 0;
}
