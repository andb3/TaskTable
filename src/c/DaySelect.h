#ifndef DAY_SELECT_H
#define DAY_SELECT_H

#include <pebble.h>
#include "DaySelect.h"
#include "alarmtime.h"
#include "common.h"
#include "commonwin.h"
#include "AddTask.h"

//const int STATUS_BAR_LAYER_HEIGHT = 20;


void day_select_window_load(Window *window);;

void day_select_window_unload(Window *window);

void day_select_init(AlarmTimeCallBack set_event);

void day_select_deinit(void);

void load_alarm();



#endif
