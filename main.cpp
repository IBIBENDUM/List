#include "list.h"
#include "list_logs.h"

int main()
{
    List list = {};
    list_init(&list);

    list.data[0] = 2;
    list.data[1] = 6;
    list.data[2] = 3;

    generate_graph(&list);

    return 0;
}
