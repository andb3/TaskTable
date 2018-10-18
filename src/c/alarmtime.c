#include "alarmtime.h"
#include "common.h"
#include "commonwin.h"
#include "AddTask.h"
#include <pebble.h>

// Screen for setting alarm times

#define LEN_HOUR 3
#define LEN_MIN 3
#define MAX_TITLE 10

// Enum for indicating which alarm time part is selected
enum part {HOUR, MINUTE};

static int8_t s_day;
static uint8_t s_hour;
static uint8_t s_minute;
static char s_hourstr[LEN_HOUR];
static char s_minutestr[LEN_MIN];
static char s_alarmtitle[MAX_TITLE];
static enum part s_selected = HOUR;
static AlarmTimeCallBack s_set_event;

static Window *s_window;
static Window *remove_window;

static GFont s_res_gothic_18_bold;
static GFont s_res_bitham_30_black;
static Layer *time_layer;

static void draw_time(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);

  graphics_context_set_text_color(ctx, GColorWhite);
  // Draw title
  graphics_draw_text(ctx, s_alarmtitle, s_res_gothic_18_bold, GRect(3+PBL_IF_ROUND_ELSE(10, 0), (bounds.size.h/2)-56, bounds.size.w-6, 37),
                     GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);

  // Draw separater
  graphics_draw_text(ctx, ":", s_res_bitham_30_black, GRect((bounds.size.w/2)-6, (bounds.size.h/2)-16, 12, 42),
                     GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);

  // Draw AM/PM indicator
  if (!clock_is_24h_style())
    graphics_draw_text(ctx, s_hour >= 12 ? "PM" : "AM", s_res_bitham_30_black, GRect((bounds.size.w/2)-26, (bounds.size.h/2)+25, 52, 37),
                     GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);

  // Set highlighted component
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_rect(ctx, GRect((bounds.size.w/2)-((s_selected == HOUR) ? 54 : -5), (bounds.size.h/2)-16, 48, 36), 0, GCornerNone);

  // Draw hour
  graphics_context_set_text_color(ctx, (s_selected == HOUR) ? GColorBlack : GColorWhite);
  graphics_draw_text(ctx, s_hourstr, s_res_bitham_30_black, GRect((bounds.size.w/2)-54, (bounds.size.h/2)-16, 48, 36),
                     GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);

  // Draw minutes
  graphics_context_set_text_color(ctx, (s_selected == MINUTE) ? GColorBlack : GColorWhite);
  graphics_draw_text(ctx, s_minutestr, s_res_bitham_30_black, GRect((bounds.size.w/2)+5, (bounds.size.h/2)-16, 48, 36),
                     GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);

  DEBUG_MSG("time drawn");


}

static void initialise_ui(void) {

  GRect bounds;
  Layer *root_layer = NULL;
  s_window = window_create_fullscreen(&root_layer, &bounds);

  s_res_gothic_18_bold = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  s_res_bitham_30_black = fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK);


  time_layer = layer_create_with_proc(root_layer, draw_time,
                                     GRect(0, 0, bounds.size.w-ACTION_BAR_WIDTH, bounds.size.h));

  DEBUG_MSG("ui init");

}

static void destroy_ui(void) {
  window_destroy(s_window);
  layer_destroy(time_layer);

}

static void handle_window_unload(Window* window) {
  destroy_ui();
}

// Redraws the currently set alarm time
static void update_alarmtime() {

  if (clock_is_24h_style()) {
    snprintf(s_hourstr, LEN_HOUR, "%d", s_hour);
  } else {
    snprintf(s_hourstr, LEN_HOUR, "%d", s_hour > 12 ? s_hour - 12 : s_hour == 0 ? 12 : s_hour);
  }
  snprintf(s_minutestr, LEN_MIN, "%.2d", s_minute);

  layer_mark_dirty(time_layer);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {

  DEBUG_MSG("select click");

  if (s_selected == HOUR) {
    // Move to the minute part
    s_selected = MINUTE;
    update_alarmtime();
  } else {
    // Close this screen
    hide_alarmtime(remove_window);
    // Pass the alarm day and time back
    s_set_event(s_day, s_hour, s_minute);
  }

}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {

  DEBUG_MSG("up click");


  if (s_selected == HOUR) {
    // Increment hour (wrap around)
    s_hour++;
    s_hour %= 24;
  } else {
    // Increment minute (wrap around)
    s_minute++;
    s_minute %= 60;
  }

  update_alarmtime();

}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {

  DEBUG_MSG("down click");


  if (s_selected == HOUR) {
    // Decrement hour (wrap around)
    s_hour += 23;
    s_hour %= 24;
  } else {
    // Decrement minute (wrap around)
    s_minute += 59;
    s_minute %= 60;
  }

  update_alarmtime();

}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_repeating_click_subscribe(BUTTON_ID_UP, 50, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
  window_single_repeating_click_subscribe(BUTTON_ID_DOWN, 50, down_click_handler);
}

void show_alarmtime(int8_t day, uint8_t hour, uint8_t minute, AlarmTimeCallBack set_event, Window *date_window) {

  DEBUG_MSG("show_alarmtime");

  initialise_ui();

  DEBUG_MSG("initialise_ui");

  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });

  DEBUG_MSG("handlers");


  s_selected = HOUR;
  // Store the passed in parameters
  s_day = day;
  s_hour = hour;
  s_minute = minute;

  remove_window = date_window;

  DEBUG_MSG("passed parameters");


  // Store pointer to callback for when done
  s_set_event = set_event;

  DEBUG_MSG("callback");


  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);


  DEBUG_MSG("Day Before: %d", tick_time->tm_mday);


  tick_time->tm_wday += s_day;

  DEBUG_MSG("Day After: %d", tick_time->tm_mday);

  strftime(s_alarmtitle, sizeof(s_alarmtitle), "%A", tick_time);

  DEBUG_MSG("daystr");





  //strncpy(s_alarmtitle, "Alarm", MAX_TITLE);


  DEBUG_MSG("screentime");


  update_alarmtime();

  DEBUG_MSG("update_alarmtime");


  window_set_click_config_provider(s_window, click_config_provider);

  DEBUG_MSG("click_config");


  window_stack_push(s_window, true);

  DEBUG_MSG("window push");

}

void hide_alarmtime(Window *date_window) {
  window_stack_remove(date_window, false);
  window_stack_remove(s_window, true);
}
