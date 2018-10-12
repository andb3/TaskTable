#include <pebble.h>
#include "TaskList.h"
#include "AddTask.h"

static Window *tasklist_window;
static Layer *s_header_layer;

static Layer *task_card_placeholder;
static int drawIndex;


static int currentRow;


static void header_update_proc(Layer *layer, GContext *ctx);
static void card_update_proc(Layer *layer, GContext *ctx);
int get_int_length(unsigned x);


void tasklist_window_unload(Window *window) {

}

void tasklist_init(int row) {

  DEBUG_MSG("tasklist window");

  currentRow = row;

  tasklist_window = window_create();
  window_set_window_handlers(tasklist_window, (WindowHandlers) {
    .load = tasklist_window_load,
    .unload = tasklist_window_unload,
  });
  const bool animated = true;
  window_stack_push(tasklist_window, animated);
}

void tasklist_deinit(void) {
  window_destroy(tasklist_window);
}

void load_table_from_tasklist(){

  DEBUG_MSG("tasklist table");


  window_stack_pop(tasklist_window);

  DEBUG_MSG("after pop");

  table_init();
}

void tasklist_window_load(Window *window) {

  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);


  // Create canvas layer
  s_header_layer = layer_create(bounds);

  // Assign the custom drawing procedure
  layer_set_update_proc(s_header_layer, header_update_proc);

  // Add to Window
  layer_add_child(window_layer, s_header_layer);

  //for(int i = 0; i<menuRows[currentRow].tasks; i++){
  for(int i = 0; i<1; i++){
    // Create canvas layer

    GRect card_bounds = GRect(
     bounds.origin.x + CARD_OFFSET_W,
     bounds.origin.y + HEADER_HEIGHT + CARD_OFFSET_H + (i * (CARD_HEIGHT + CARD_OFFSET_H)),
     bounds.size.w - (2 * CARD_OFFSET_W),
     CARD_HEIGHT
   );

    task_card_placeholder = layer_create(card_bounds);

    //drawIndex++;

    // Assign the custom drawing procedure
    layer_set_update_proc(task_card_placeholder, card_update_proc);

    // Add to Window
    layer_add_child(window_layer, task_card_placeholder);

  }

}

static void card_update_proc(Layer *layer, GContext *ctx) {

  GRect layer_bounds = layer_get_bounds(layer);
  GRect top_bounds =  GRect(layer_bounds.origin.x+2, layer_bounds.origin.y+2, layer_bounds.size.w-4, layer_bounds.size.h-4);


  #ifdef PBL_COLOR
  // Set the fill color
  graphics_context_set_fill_color(ctx, GColorLightGray);
  #else
  // Set the fill color
  graphics_context_set_fill_color(ctx, GColorBlack);
  #endif

  graphics_fill_rect(ctx, layer_bounds, 0, GCornersAll);

  graphics_context_set_fill_color(ctx, GColorWhite);

  graphics_fill_rect(ctx, top_bounds, 0, GCornersAll);

  // Set the font color
  graphics_context_set_text_color(ctx, GColorBlack);

  GFont font = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);



  int tasksLength = get_int_length(menuRows[currentRow].tasks);

  DEBUG_MSG("Int length: %d", tasksLength);

  char *number_buffer = malloc(sizeof(char) *  (tasksLength + 1));
  dec_to_str(number_buffer, menuRows[currentRow].tasks, tasksLength);

  char *text = malloc(sizeof(char) * (strlen("Tasks:") + tasksLength + 1));
  text = "Tasks:";

  strcat(text, number_buffer);

  DEBUG_MSG("Final text: %s", text);


  // Determine a reduced bounding box
  GRect text_bounds = GRect(top_bounds.origin.x, top_bounds.origin.y + (CARD_HEIGHT/4),
                       layer_bounds.size.w, (CARD_HEIGHT/2));


  // Draw the text
  graphics_draw_text(ctx, text, font, text_bounds, GTextOverflowModeWordWrap,
                                              GTextAlignmentLeft, NULL);

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


  //char *text = "Task List";
  char *text = malloc(sizeof(char) * (strlen(menuRows[currentRow].name) + 1));
  text = menuRows[currentRow].name;


  // Determine a reduced bounding box
  GRect bounds = GRect(layer_bounds.origin.x, layer_bounds.origin.y + (HEADER_HEIGHT/4),
                       layer_bounds.size.w, (HEADER_HEIGHT/2));

  // Calculate the size of the text to be drawn, with restricted space
  //GSize text_size = graphics_text_layout_get_content_size(text, font, bounds, GTextOverflowModeWordWrap, GTextAlignmentCenter);

  // Draw the text
  graphics_draw_text(ctx, text, font, bounds, GTextOverflowModeWordWrap,
                                              GTextAlignmentCenter, NULL);



}


int get_int_length(unsigned x) {
    if (x >= 100)        return 3;
    if (x >= 10)         return 2;
    return 1;
}
