#include <pebble.h>
#include "DaySelect.h"
#include "alarmtime.h"
#include "common.h"
#include "commonwin.h"
#include "AddTask.h"



static Window *day_select_window;

static void day_select_click_config(void *context);

static AlarmTimeCallBack set_event;


void day_select_window_load(Window *window) {
  //load_alarm();
}

void day_select_window_unload(Window *window) {

}

static void day_select_click_config(void *context)
{
  //window_single_click_subscribe(BUTTON_ID_UP, desc_button_up);
  window_single_click_subscribe(BUTTON_ID_SELECT, (ClickHandler) load_alarm);
  //window_single_click_subscribe(BUTTON_ID_DOWN, time_button_down);
}

void day_select_init(AlarmTimeCallBack got_callback) {

  DEBUG_MSG("day_select window");

  set_event = got_callback;


  day_select_window = window_create();
  window_set_window_handlers(day_select_window, (WindowHandlers) {
    .load = day_select_window_load,
    .unload = day_select_window_unload,
  });
  const bool animated = true;
  window_set_click_config_provider(day_select_window, day_select_click_config);
  window_stack_push(day_select_window, animated);

  //show_alarmtime(1, 8, 30, set_event);


}

void day_select_deinit(void) {
  window_destroy(day_select_window);
}

void load_alarm(){

  DEBUG_MSG("day_select table");



  DEBUG_MSG("after pop");

  show_alarmtime(1, 8, 30, set_event);
}
