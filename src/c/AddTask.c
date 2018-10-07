#include <pebble.h>
#include "TimeTable.h"
#include "TaskTable.h"
#include "AddTask.h"
#include "alarmtime.h"
#include "common.h"
#include "commonwin.h"




static Window *add_task_window;
static TextLayer *add_task_text_layer;
static Layer *s_header_layer;
static Layer *s_footer_layer;

static int currentRow;

static char* task_text = "No Description";
static int daysInFuture = 0;
static int hourOfDay = 8;
static int minuteOfHour = 30;
static int task_time = 0;


static ActionBarLayer *s_action_bar;
static GBitmap *s_up_bitmap, *s_down_bitmap, *s_check_bitmap;

static void add_task_click_config();
void desc_button_up();
void add_button_select();
void time_button_down();
static void header_update_proc(Layer *layer, GContext *ctx);
static void footer_update_proc(Layer *layer, GContext *ctx);




void add_task_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  GRect text_bounds = GRect(bounds.origin.x, bounds.origin.y+ HEADER_HEIGHT, bounds.size.w, bounds.size.h-HEADER_HEIGHT);

  add_task_text_layer = text_layer_create(text_bounds);
  text_layer_set_text(add_task_text_layer, task_text);
  text_layer_set_text_alignment(add_task_text_layer, GTextAlignmentLeft);

  layer_add_child(window_layer, text_layer_get_layer(add_task_text_layer));


  #if defined(PBL_ROUND)

  text_layer_enable_screen_text_flow_and_paging(add_task_text_layer, 5);

  // Set the ScrollLayer's content size to the total size of the text

  //scroll_layer_set_content_size(thread_scroll_layer,

  //text_layer_get_content_size(thread_body_layer));

  // Enable ScrollLayer paging

  //scroll_layer_set_paging(thread_scroll_layer, true);

  /*s_indicator = scroll_layer_get_content_indicator(comment_scroll_layer);

  s_indicator_up_layer = layer_create(GRect(0,0,bounds.size.w, STATUS_BAR_LAYER_HEIGHT));

  s_indicator_down_layer = layer_create(GRect(0,bounds.size.h - STATUS_BAR_LAYER_HEIGHT, bounds.size.w, STATUS_BAR_LAYER_HEIGHT));

  layer_add_child(window_get_root_layer(window), s_indicator_up_layer);

  layer_add_child(window_get_root_layer(window), s_indicator_down_layer);*/

  #endif

  layer_add_child(window_layer, text_layer_get_layer(add_task_text_layer));

  // Create canvas layer
  s_header_layer = layer_create(bounds);

  // Assign the custom drawing procedure
  layer_set_update_proc(s_header_layer, header_update_proc);

  // Add to Window
  layer_add_child(window_layer, s_header_layer);



  // Load icon bitmaps
  s_up_bitmap = gbitmap_create_with_resource(RESOURCE_ID_UP_ICON);
  s_down_bitmap = gbitmap_create_with_resource(RESOURCE_ID_DOWN_ICON);
  s_check_bitmap = gbitmap_create_with_resource(RESOURCE_ID_CHECK_ICON);

  // Create ActionBarLayer
  s_action_bar = action_bar_layer_create();
  action_bar_layer_set_click_config_provider(s_action_bar, add_task_click_config);

  // Set the icons
  action_bar_layer_set_icon(s_action_bar, BUTTON_ID_UP, s_up_bitmap);
  action_bar_layer_set_icon(s_action_bar, BUTTON_ID_DOWN, s_down_bitmap);
  action_bar_layer_set_icon(s_action_bar, BUTTON_ID_SELECT, s_check_bitmap);

  // Add to Window
  action_bar_layer_add_to_window(s_action_bar, add_task_window);




}

static void header_update_proc(Layer *layer, GContext *ctx) {
  // Custom drawing happens here!

  GRect layer_bounds = layer_get_bounds(layer);

  #ifdef PBL_COLOR

  // Set the fill color
  graphics_context_set_fill_color(ctx, GColorPictonBlue);

  // Set the font color
  graphics_context_set_text_color(ctx, GColorBlack);

  #else

  // Set the fill color
  graphics_context_set_fill_color(ctx, GColorBlack);

  // Set the font color
  graphics_context_set_text_color(ctx, GColorWhite);

  #endif


  GRect rect_bounds = GRect(layer_bounds.origin.x, layer_bounds.origin.y, layer_bounds.size.w, HEADER_HEIGHT);


  // Draw a rectangle
  //graphics_draw_rect(ctx, rect_bounds);

  graphics_fill_rect(ctx, rect_bounds, 0, GCornersAll);


  // Load the font
  GFont font = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);


  char *text = "Add a Task";

  // Determine a reduced bounding box
  GRect bounds = GRect(layer_bounds.origin.x, layer_bounds.origin.y + (HEADER_HEIGHT/4),
                       layer_bounds.size.w, (HEADER_HEIGHT/2));

  // Calculate the size of the text to be drawn, with restricted space
  //GSize text_size = graphics_text_layout_get_content_size(text, font, bounds, GTextOverflowModeWordWrap, GTextAlignmentCenter);

  // Draw the text
  graphics_draw_text(ctx, text, font, bounds, GTextOverflowModeWordWrap,
                                              GTextAlignmentCenter, NULL);



}

static void footer_update_proc(Layer *layer, GContext *ctx) {
  // Custom drawing happens here!

  GRect layer_bounds = layer_get_bounds(layer);

  #ifdef PBL_COLOR

  // Set the fill color
  graphics_context_set_fill_color(ctx, GColorPictonBlue);

  // Set the font color
  graphics_context_set_text_color(ctx, GColorBlack);

  #else

  // Set the fill color
  graphics_context_set_fill_color(ctx, GColorBlack);

  // Set the font color
  graphics_context_set_text_color(ctx, GColorWhite);

  #endif


  GRect rect_bounds = GRect(layer_bounds.origin.x, layer_bounds.size.h-HEADER_HEIGHT, layer_bounds.size.w, HEADER_HEIGHT);


  // Draw a rectangle
  //graphics_draw_rect(ctx, rect_bounds);

  graphics_fill_rect(ctx, rect_bounds, 0, GCornersAll);


  // Load the font
  //GFont font = fonts_get_system_font(FONT_KEY_LECO_26_BOLD_NUMBERS_AM_PM);
  GFont font = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);



  char text[9] = "";
  char *number_buffer = malloc(sizeof(char)* (3));
  bool pm = false;

  DEBUG_MSG("Declarations: %c", text[0]);

  if(hourOfDay>12){
    dec_to_str(number_buffer, hourOfDay-12, 2);
    if(hourOfDay != 24){
      pm = true;
    }

  }else{
    dec_to_str(number_buffer, hourOfDay, 2);
  }

  DEBUG_MSG("hour conversion: %s", number_buffer);

  strcat(text, number_buffer);
  strcat(text, " ");

  DEBUG_MSG("hour strcat: %c", text[0]);


  dec_to_str(number_buffer, minuteOfHour, 2);

  DEBUG_MSG("minute conversion");

  strcat(text, number_buffer);
  strcat(text, " ");

  DEBUG_MSG("minute strcat");


  if(pm){
    strcat(text, "PM");
  }else{
    strcat(text, "AM");

  }

  DEBUG_MSG("am/pm strcat");
  DEBUG_MSG("final: %s", text);



  //text = "Test Time";


  // Determine a reduced bounding box
  GRect bounds = GRect(layer_bounds.origin.x, layer_bounds.size.h - HEADER_HEIGHT,
                       layer_bounds.size.w, (HEADER_HEIGHT/2));

  DEBUG_MSG("bounding");


  // Calculate the size of the text to be drawn, with restricted space
  //GSize text_size = graphics_text_layout_get_content_size(text, font, bounds, GTextOverflowModeWordWrap, GTextAlignmentCenter);

  // Draw the text
  graphics_draw_text(ctx, text, font, bounds, GTextOverflowModeWordWrap,
                                              GTextAlignmentCenter, NULL);

  DEBUG_MSG("drawn");


}


static void add_task_click_config(void *context)
{
  window_single_click_subscribe(BUTTON_ID_UP, desc_button_up);
  window_single_click_subscribe(BUTTON_ID_SELECT, (ClickHandler) add_button_select);
  window_single_click_subscribe(BUTTON_ID_DOWN, time_button_down);
}

void desc_button_up(){

}
void add_button_select(){

  DictionaryIterator *index;

  if(app_message_outbox_begin(&index) == APP_MSG_OK) {

    dict_write_uint8(index, MESSAGE_KEY_TaskAddIndex, (uint8_t)currentRow);

    DEBUG_MSG("Index: %d", currentRow);

    app_message_outbox_send();
  }

  DictionaryIterator *desc;

  if(app_message_outbox_begin(&desc) == APP_MSG_OK) {

    dict_write_cstring(desc, MESSAGE_KEY_TaskAddDescription, /*(cstring_t)*/task_text);

    DEBUG_MSG("Text: %s", task_text);

    app_message_outbox_send();
  }

  DictionaryIterator *tasktime;

  if(app_message_outbox_begin(&tasktime) == APP_MSG_OK) {

    dict_write_uint8(tasktime, MESSAGE_KEY_TaskAddTime, (uint8_t)task_time);

    DEBUG_MSG("Time: %d", task_time);

    app_message_outbox_send();
  }


}
void time_button_down(){
  show_alarmtime(1, 8, 30, return_time);
}

void return_time(int8_t day, uint8_t hour, uint8_t minute){

  //DEBUG_MSG("return time");

  daysInFuture = day;
  hourOfDay = hour;
  minuteOfHour = minute;


  //Layer *window_layer = action_bar_layer_get_layer(s_action_bar);


  Layer *window_layer = window_get_root_layer(add_task_window);
  GRect bounds = layer_get_bounds(window_layer);

  // Create canvas layer
  s_footer_layer = layer_create(bounds);

  // Assign the custom drawing procedure
  layer_set_update_proc(s_footer_layer, footer_update_proc);

  // Add to Window
  layer_add_child(window_layer, s_footer_layer);

}




void add_task_window_unload(Window *window) {
  //menu_layer_destroy(add_task_text_layer);
  // Destroy the ActionBarLayer
  action_bar_layer_destroy(s_action_bar);

  // Destroy the icon GBitmaps
  gbitmap_destroy(s_up_bitmap);
  gbitmap_destroy(s_down_bitmap);
  gbitmap_destroy(s_check_bitmap);
}

void add_task_init(int passedRow) {

  currentRow = passedRow;

  add_task_window = window_create();
  window_set_window_handlers(add_task_window, (WindowHandlers) {
    .load = add_task_window_load,
    .unload = add_task_window_unload,
  });
  const bool animated = true;
  window_stack_push(add_task_window, animated);
}

void add_task_deinit(void) {
  window_destroy(add_task_window);
}
