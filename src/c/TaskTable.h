#ifndef TASK_TABLE_H
#define TASK_TABLE_H

#include <pebble.h>
#include "TimeTable.h"
#include "AppMessages.h"

//extern bool table_loaded;



void table_window_load(Window *window);;

void table_window_unload(Window *window);

void table_init(void);

void table_deinit(void);

#endif
