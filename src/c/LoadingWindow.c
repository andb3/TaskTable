#include <pebble.h>
#include "TaskTable.h"
#include "AddTask.h"

static Window *loading_window;


void loading_window_load(Window *window) {

}

void loading_window_unload(Window *window) {

}

void loading_init(void) {

  DEBUG_MSG("loading window");


  loading_window = window_create();
  window_set_window_handlers(loading_window, (WindowHandlers) {
    .load = loading_window_load,
    .unload = loading_window_unload,
  });
  const bool animated = true;
  window_stack_push(loading_window, animated);
}

void loading_deinit(void) {
  window_destroy(loading_window);
}

void load_table(){

  DEBUG_MSG("loading table");


  window_stack_pop(loading_window);

  DEBUG_MSG("after pop");

  table_init();
}
