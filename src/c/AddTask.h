#ifndef ADD_TASK_H
#define ADD_TASK_H

#include <pebble.h>
#include "TimeTable.h"


static int HEADER_HEIGHT = 40;


void add_task_window_load(Window *window);;

void add_task_window_unload(Window *window);

void add_task_init(int passedRow);

void add_task_deinit(void);

void return_time(int8_t day, uint8_t hour, uint8_t minute);


#define DEBUG_MODE

#ifdef DEBUG_MODE
	#define DEBUG_MSG(args...) APP_LOG(APP_LOG_LEVEL_DEBUG, args)
#else
	#define DEBUG_MSG(args...)
#endif

#endif
