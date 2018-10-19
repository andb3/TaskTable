#include <pebble.h>
#include "DaySelect.h"
#include "alarmtime.h"
#include "common.h"
#include "commonwin.h"
#include "AddTask.h"



static Window *day_select_window;
static MenuLayer *day_select_menu_layer;

static Layer *s_indicator_up_layer;
static Layer *s_indicator_down_layer;
static ContentIndicator *s_indicator_up;
static ContentIndicator *s_indicator_down;

//static void day_select_click_config(void *context);

static AlarmTimeCallBack set_event;

static MenuLayer *day_select_menu_layer;


static char *dayNames[3] = {"Today", "Tomorrow", "Next week"};

static uint16_t get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data);
static void draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data);
static void select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data);
static int16_t get_cell_height_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *context);



void day_select_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  day_select_menu_layer = menu_layer_create(bounds);

  menu_layer_set_highlight_colors(day_select_menu_layer, GColorWhite, GColorBlack);

  menu_layer_set_click_config_onto_window(day_select_menu_layer, window);
  menu_layer_set_callbacks(day_select_menu_layer, NULL, (MenuLayerCallbacks) {
      .get_num_rows = get_num_rows_callback,
      .draw_row = draw_row_callback,
      .get_cell_height = get_cell_height_callback,
      .select_click = select_callback,
      //.select_long_click = long_callback,
  });
  //text_layer_set_text(day_select_menu_layer, "Press a button");
  //text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);


  layer_add_child(window_layer, menu_layer_get_layer(day_select_menu_layer));


  // Create two Layers to draw the arrows
  s_indicator_up_layer = layer_create(
                          GRect(0, 0, bounds.size.w, STATUS_BAR_LAYER_HEIGHT));
  s_indicator_down_layer = layer_create(
                          GRect(0, bounds.size.h - STATUS_BAR_LAYER_HEIGHT,
                                bounds.size.w, STATUS_BAR_LAYER_HEIGHT));


  s_indicator_up = content_indicator_create();
  const ContentIndicatorConfig up_config = (ContentIndicatorConfig) {
    .layer = s_indicator_up_layer,
    .times_out = false,
    .alignment = GAlignCenter,
    .colors = {
      .foreground = GColorBlack,
      .background = GColorWhite
    }
  };
  content_indicator_configure_direction(s_indicator_up, ContentIndicatorDirectionUp,
                                      &up_config);
  s_indicator_down = content_indicator_create();
  const ContentIndicatorConfig down_config = (ContentIndicatorConfig) {
    .layer = s_indicator_down_layer,
    .times_out = false,
    .alignment = GAlignCenter,
    .colors = {
      .foreground = GColorBlack,
      .background = GColorWhite
    }
  };
  content_indicator_configure_direction(s_indicator_down, ContentIndicatorDirectionDown,
                                        &down_config);

  content_indicator_set_content_available(s_indicator_down, ContentIndicatorDirectionDown, true);
  content_indicator_set_content_available(s_indicator_up, ContentIndicatorDirectionUp, false);



  /* ... */

  // Add these Layers as children after all other components to appear below
  layer_add_child(window_layer, s_indicator_up_layer);
  layer_add_child(window_layer, s_indicator_down_layer);
}



static uint16_t get_num_rows_callback(MenuLayer *menu_layer,
                                      uint16_t section_index, void *context) {
  const uint16_t num_rows = 8;
  return num_rows;
}

static void draw_row_callback(GContext *ctx, const Layer *cell_layer,
                                        MenuIndex *cell_index, void *context) {
  //static char s_buff[16];
  //snprintf(s_buff, sizeof(s_buff), "Row %d", (int)cell_index->row);

  GFont font = fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK	);
  GRect layer_bounds = layer_get_bounds(cell_layer);
  GRect bounds = GRect(layer_bounds.origin.x, layer_bounds.size.h/2 - 20,
                       layer_bounds.size.w, layer_bounds.size.h/2);

  DEBUG_MSG("Row %d", (int)cell_index->row);

  if((int)cell_index->row<2){
    // Draw this row's index
    //menu_cell_basic_draw(ctx, cell_layer, dayNames[(int)cell_index->row], NULL, NULL);


    // Calculate the size of the text to be drawn, with restricted space
    GSize text_size = graphics_text_layout_get_content_size(dayNames[(int)cell_index->row], font, bounds,
                                  GTextOverflowModeWordWrap, GTextAlignmentCenter);

    graphics_draw_text(ctx, dayNames[(int)cell_index->row], font, bounds, GTextOverflowModeWordWrap,
                                            GTextAlignmentCenter, NULL);
  }else if((int)cell_index->row==7){
    //menu_cell_basic_draw(ctx, cell_layer, dayNames[2], NULL, NULL);


    // Calculate the size of the text to be drawn, with restricted space
    GSize text_size = graphics_text_layout_get_content_size(dayNames[2], font, bounds,
                                  GTextOverflowModeWordWrap, GTextAlignmentCenter);

    graphics_draw_text(ctx, dayNames[2], font, bounds, GTextOverflowModeWordWrap,
                                            GTextAlignmentCenter, NULL);
  }else{

    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp);


    DEBUG_MSG("Day Before: %d", tick_time->tm_mday);


    tick_time->tm_wday += (int)cell_index->row;

    DEBUG_MSG("Day After: %d", tick_time->tm_mday);

    char dayText[10];
    strftime(dayText, sizeof(dayText), "%A", tick_time);

    //menu_cell_basic_draw(ctx, cell_layer, dayText, NULL, NULL);


    //free(dayText);


    // Calculate the size of the text to be drawn, with restricted space
    GSize text_size = graphics_text_layout_get_content_size(dayText, font, bounds,
                                  GTextOverflowModeWordWrap, GTextAlignmentCenter);

    graphics_draw_text(ctx, dayText, font, bounds, GTextOverflowModeWordWrap,
                                            GTextAlignmentCenter, NULL);
  }


  //TODO: Next week lead to date select ("select date"?)


  if(menu_cell_layer_is_highlighted(cell_layer)){
    if((int)cell_index->row==0){
      content_indicator_set_content_available(s_indicator_down, ContentIndicatorDirectionDown, true);
      content_indicator_set_content_available(s_indicator_up, ContentIndicatorDirectionUp, false);
    }else if((int)cell_index->row==7){
      content_indicator_set_content_available(s_indicator_down, ContentIndicatorDirectionDown, false);
      content_indicator_set_content_available(s_indicator_up, ContentIndicatorDirectionUp, true);
    }else{

    content_indicator_set_content_available(s_indicator_down, ContentIndicatorDirectionDown, true);
    content_indicator_set_content_available(s_indicator_up, ContentIndicatorDirectionUp, true);
    }
  }

}

static int16_t get_cell_height_callback(struct MenuLayer *menu_layer,
                                        MenuIndex *cell_index, void *context) {

  Layer *window_layer = window_get_root_layer(day_select_window);
  GRect bounds = layer_get_bounds(window_layer);

  //const int16_t cell_height = 104;
  const int16_t cell_height = bounds.size.h;

  return cell_height;
}

static void select_callback(struct MenuLayer *menu_layer,
                                        MenuIndex *cell_index, void *context) {
  // Do something in response to the button press
  //window_stack_pop(true);
  //add_task_init((int)cell_index->row);


    //window_stack_pop(true);
    load_alarm((int)cell_index->row);


}




void day_select_window_unload(Window *window) {

}

/*static void day_select_click_config(void *context)
{
  //window_single_click_subscribe(BUTTON_ID_UP, desc_button_up);
  window_single_click_subscribe(BUTTON_ID_SELECT, (ClickHandler) load_alarm);
  //window_single_click_subscribe(BUTTON_ID_DOWN, time_button_down);
}*/

void day_select_init(AlarmTimeCallBack got_callback) {

  DEBUG_MSG("day_select window");

  set_event = got_callback;


  day_select_window = window_create();
  window_set_window_handlers(day_select_window, (WindowHandlers) {
    .load = day_select_window_load,
    .unload = day_select_window_unload,
  });
  const bool animated = true;
  //window_set_click_config_provider(day_select_window, day_select_click_config);
  window_stack_push(day_select_window, animated);

  //show_alarmtime(1, 8, 30, set_event);


}

void day_select_deinit(void) {
  window_destroy(day_select_window);
  content_indicator_destroy(s_indicator_up);
  content_indicator_destroy(s_indicator_down);
}

void load_alarm(int day){

  DEBUG_MSG("day_select table");



  DEBUG_MSG("after pop");

  show_alarmtime(day, 8, 30, set_event, day_select_window);
}
