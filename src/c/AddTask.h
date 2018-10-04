#ifndef ADD_TASK_H
#define ADD_TASK_H

#include <pebble.h>
#include "TimeTable.h"


static int HEADER_HEIGHT = 40;


void add_task_window_load(Window *window);;

void add_task_window_unload(Window *window);

void add_task_init(int passedRow);

void add_task_deinit(void);

void return_time(int s_day, int s_hour, int s_minute);

#endif
