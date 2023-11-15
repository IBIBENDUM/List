#ifndef LIST_LOGS_H
#define LIST_LOGS_H

#include "list.h"

const int BG_COLOR      = 0xFFFFFF;
const int FILL_COLOR    = 0xFFE4C4;
const int OUTLINE_COLOR = 0x800000;
const int NEXT_COLOR    = 0x00FF00;
const int PREV_COLOR    = 0x00FFFF;

list_error generate_graph(List* list);

#endif
