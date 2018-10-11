#ifndef LOADING_WINDOW_H
#define LOADING_WINDOW_H

#include <pebble.h>
#include "TimeTable.h"
#include "AppMessages.h"
#include "TaskTable.h"

//extern bool loading_loaded;



void loading_window_load(Window *window);;

void loading_window_unload(Window *window);

void loading_init(void);

void loading_deinit(void);

void load_table();

#endif
