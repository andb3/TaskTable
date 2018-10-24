#include <pebble.h>
#include <stdio.h>
#include "TimeTable.h"
#include "TaskTable.h"
#include "AddTask.h"
#include "alarmtime.h"
#include "common.h"
#include "commonwin.h"
#include "AppMessages.h"
#include "DaySelect.h"
#include "tertiary_text.h"






static Window *add_task_window;
static TextLayer *add_task_text_layer;
static Layer *s_header_layer;
static Layer *s_footer_layer;

static int currentRow;

static char* task_text;
static int daysInFuture = 0;
static int hourOfDay = 8;
static int minuteOfHour = 30;
static int task_time = 0;


static ActionBarLayer *s_action_bar;
static GBitmap *s_up_bitmap, *s_down_bitmap, *s_check_bitmap;
static DictationSession *add_task_dictation_session;


static void add_task_click_config();
void desc_button_up();
void add_button_select();
void time_button_down();
static void header_update_proc(Layer *layer, GContext *ctx);
static void footer_update_proc(Layer *layer, GContext *ctx);
static void dictation_session_callback(DictationSession *session, DictationSessionStatus status, char *transcription, void *context);
void type_button_up();
void type_callback(const char* result, size_t result_length, void* extra);



void add_task_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  GRect text_bounds = GRect(bounds.origin.x, bounds.origin.y+ HEADER_HEIGHT, bounds.size.w, bounds.size.h-HEADER_HEIGHT);

  add_task_text_layer = text_layer_create(text_bounds);
  //task_text = malloc(sizeof(char) * (strlen("No Description") + 1));
  //strcpy(task_text, "No Description");
  //text_layer_set_text(add_task_text_layer, task_text);
  text_layer_set_text(add_task_text_layer, "No Description");
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
  strcat(text, ":");

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
  window_long_click_subscribe(BUTTON_ID_UP, 1000, type_button_up, NULL);
  window_single_click_subscribe(BUTTON_ID_SELECT, (ClickHandler) add_button_select);
  window_single_click_subscribe(BUTTON_ID_DOWN, time_button_down);
}

void desc_button_up(){
  // Create new dictation session

  int buffer_size = 128;

  #if defined(PBL_ROUND)

  buffer_size = 512;

  #endif



  add_task_dictation_session = dictation_session_create(sizeof(char) * buffer_size,
                                                 dictation_session_callback, NULL);

  // Start dictation UI
  dictation_session_start(add_task_dictation_session);


}

void type_button_up() {
  tertiary_text_prompt("Task Description", type_callback, NULL);
}
void type_callback(const char* result, size_t result_length, void* extra) {
  task_text = malloc(sizeof(char) * (result_length + 1));

  //DEBUG_MSG("Status: %d", status);


    strcpy(task_text, result);
    text_layer_set_text(add_task_text_layer, task_text);
}

void add_button_select(){

  DEBUG_MSG("Select pressed");

  // Calculate the buffer size that is needed for the final Dictionary:
  //const uint8_t key_count = 3;
  //const uint32_t size = dict_calc_buffer_size(key_count, sizeof(currentRow), strlen(task_text) + 1, sizeof(task_time));

  // Stack-allocated buffer in which to create the Dictionary:
  //uint8_t buffer[size];


  DEBUG_MSG("init appmessage");

  DEBUG_MSG("Days: %d", daysInFuture);
  DEBUG_MSG("Hours: %d", hourOfDay);
  DEBUG_MSG("Minutes: %d", minuteOfHour);

  task_time = daysInFuture*10000;
  task_time += hourOfDay*100;
  task_time += minuteOfHour;

  DEBUG_MSG("Time: %d", task_time);

  DictionaryIterator *iter;

  DEBUG_MSG("iter created");

  //app_message_outbox_begin(&iter);

  if(app_message_outbox_begin(&iter) == APP_MSG_OK) {

    DEBUG_MSG("ok appmessage");


    dict_write_uint8(iter, MESSAGE_KEY_TaskAddIndex, (uint8_t)currentRow);

    DEBUG_MSG("index");


    dict_write_cstring(iter, MESSAGE_KEY_TaskAddDescription, task_text);

    DEBUG_MSG("desc");

    dict_write_uint32(iter, MESSAGE_KEY_TaskAddTime, (uint32_t)task_time);

    DEBUG_MSG("written");


    DEBUG_MSG("Index: %d", currentRow);
    DEBUG_MSG("Text: %s", task_text);
    DEBUG_MSG("Time: %d", task_time);

    DEBUG_MSG("debug");

    if(!comm_is_js_ready()){
      return;
    }


    // Send this message
    if(app_message_outbox_send() != APP_MSG_OK) {
      DEBUG_MSG("Error sending the outbox");
    }else{
      DEBUG_MSG("sent");

      setMenuCount(menuRows[currentRow].tasks + 1, currentRow);

      window_stack_pop(add_task_window);
      //free(task_text);
      DEBUG_MSG("free");

    }
  } else {
    // The outbox cannot be used right now
    DEBUG_MSG("Error preparing the outbox");
  }


}
void time_button_down(){

  //show_alarmtime(1, 8, 30, return_time);


  day_select_init(return_time);
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

  action_bar_layer_remove_from_window(s_action_bar);
  action_bar_layer_set_click_config_provider(s_action_bar, add_task_click_config);
  action_bar_layer_add_to_window(s_action_bar, add_task_window);


}

static void dictation_session_callback(DictationSession *session, DictationSessionStatus status, char *transcription, void *context) {

  if(transcription == NULL){
    //transcription = malloc(sizeof(char) * (strlen("No Description") + 1));
    transcription = malloc(sizeof(char) * (1));
    transcription = "";
    DEBUG_MSG("Null transcription");
  }

  task_text = malloc(sizeof(char) * (strlen(transcription) + 1));

  //DEBUG_MSG("Status: %d", status);

  if(status==DictationSessionStatusSuccess){
    strcpy(task_text, transcription);
    text_layer_set_text(add_task_text_layer, task_text);
    //text_layer_set_text(add_task_text_layer, "Transcription");
    //layer_mark_dirty(add_task_text_layer);
  }
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
