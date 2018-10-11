#include <pebble.h>
#include "TimeTable.h"
#include "TaskTable.h"
#include "AddTask.h"
#include "AppMessages.h"

static Window *s_window;
static MenuLayer *s_menu_layer;


bool table_loaded;

static uint16_t get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data);
static void draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data);
static void select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data);


static uint16_t get_num_rows_callback(MenuLayer *menu_layer,
                                      uint16_t section_index, void *context) {
  const uint16_t num_rows = table_size;
  return num_rows;
}

static void draw_row_callback(GContext *ctx, const Layer *cell_layer,
                                        MenuIndex *cell_index, void *context) {
  static char s_buff[16];
  snprintf(s_buff, sizeof(s_buff), "Row %d", (int)cell_index->row);

  // Draw this row's index
  menu_cell_basic_draw(ctx, cell_layer, menuRows[(int)cell_index->row].name, NULL, NULL);
}

static int16_t get_cell_height_callback(struct MenuLayer *menu_layer,
                                        MenuIndex *cell_index, void *context) {
  const int16_t cell_height = 44;
  return cell_height;
}

static void select_callback(struct MenuLayer *menu_layer,
                                        MenuIndex *cell_index, void *context) {
  // Do something in response to the button press
  //window_stack_pop(true);
  add_task_init((int)cell_index->row);

}


void table_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_menu_layer = menu_layer_create(bounds);
  menu_layer_set_click_config_onto_window(s_menu_layer, window);
  menu_layer_set_callbacks(s_menu_layer, NULL, (MenuLayerCallbacks) {
      .get_num_rows = get_num_rows_callback,
      .draw_row = draw_row_callback,
      .get_cell_height = get_cell_height_callback,
      .select_click = select_callback,
  });
  //text_layer_set_text(s_menu_layer, "Press a button");
  //text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, menu_layer_get_layer(s_menu_layer));
}

void table_window_unload(Window *window) {
  menu_layer_destroy(s_menu_layer);
}

void table_init(void) {

  setMenuRows();

  s_window = window_create();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = table_window_load,
    .unload = table_window_unload,
  });
  const bool animated = true;
  window_stack_push(s_window, animated);
}

void table_deinit(void) {
  window_destroy(loading_window);
}



int main(void) {
  //table_loaded = false;
  app_message_init();

  //while (!table_loaded) {
    //DEBUG_MSG("table not loaded");
  //}



  //APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", s_window);

  app_event_loop();
  table_deinit();
}
