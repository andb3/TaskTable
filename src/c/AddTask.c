#include <pebble.h>
#include "TimeTable.h"
#include "TaskTable.h"
#include "AddTask.h"


static Window *add_task_window;
static TextLayer *add_task_text_layer;
static Layer *s_header_layer;


static ActionBarLayer *s_action_bar;
static GBitmap *s_up_bitmap, *s_down_bitmap, *s_check_bitmap;

static void add_task_click_config();
void desc_button_up();
void add_button_select();
void time_button_down();
static void header_update_proc(Layer *layer, GContext *ctx);



void add_task_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  GRect text_bounds = GRect(bounds.origin.x, bounds.origin.y+ HEADER_HEIGHT, bounds.size.w, bounds.size.h-HEADER_HEIGHT);

  add_task_text_layer = text_layer_create(text_bounds);
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


  // Create canvas layer
  s_header_layer = layer_create(bounds);

  // Assign the custom drawing procedure
  layer_set_update_proc(s_header_layer, header_update_proc);

  // Add to Window
  layer_add_child(window_get_root_layer(window), s_header_layer);

}

static void header_update_proc(Layer *layer, GContext *ctx) {
  // Custom drawing happens here!

  GRect layer_bounds = layer_get_bounds(layer);

  // Set the fill color
  graphics_context_set_fill_color(ctx, GColorBlue);
  GRect rect_bounds = GRect(layer_bounds.origin.x, layer_bounds.origin.y, layer_bounds.size.w, HEADER_HEIGHT);
  graphics_fill_rect(ctx, rect_bounds, 0, GCornersAll);


  // Draw a rectangle
  graphics_draw_rect(ctx, rect_bounds);

  // Load the font
  GFont font = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
  // Set the color
  graphics_context_set_text_color(ctx, GColorBlack);

  char *text = "Add a Task";

  // Determine a reduced bounding box
  GRect bounds = GRect(layer_bounds.origin.x, layer_bounds.origin.y,
                       layer_bounds.size.w, HEADER_HEIGHT);

  // Calculate the size of the text to be drawn, with restricted space
  //GSize text_size = graphics_text_layout_get_content_size(text, font, bounds, GTextOverflowModeWordWrap, GTextAlignmentCenter);

  // Draw the text
  graphics_draw_text(ctx, text, font, bounds, GTextOverflowModeWordWrap,
                                              GTextAlignmentCenter, NULL);



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

}
void time_button_down(){

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

void add_task_init(void) {

  setMenuRows();

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
