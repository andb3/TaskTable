#ifndef TASK_LIST_H
#define TASK_LIST_H

#include <pebble.h>
#include "TimeTable.h"
#include "AppMessages.h"
#include "TaskTable.h"

static int CARD_HEIGHT = 44;
static int CARD_OFFSET_W = 20;
static int CARD_OFFSET_H = 10;

void tasklist_window_load(Window *window);

void tasklist_window_unload(Window *window);

void tasklist_init(int row);

void tasklist_deinit(void);

void load_table_from_tasklist();

int get_int_length(unsigned x);


#endif
