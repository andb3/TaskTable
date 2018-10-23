#include <pebble.h>
#include <@smallstoneapps/linked-list/linked-list.h>
#include "TaskList.h"
#include "TimeTable.h"
#include "AddTask.h"

static Window *tasklist_window;
static Layer *s_header_layer;
static MenuLayer *task_menu_layer;


int tasksLength;
char *text;

static int currentRow;

static void header_update_proc(Layer *layer, GContext *ctx);
static void update_task_count();
static void tasklist_button_select();

static uint16_t get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data);
static void draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data);
static void select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data);
static int16_t get_cell_height_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *context);


void tasklist_window_unload(Window *window) {

  DEBUG_MSG("before free");
  text = NULL;

  free(text);

  DEBUG_MSG("after free");


}

void tasklist_init(int row) {

  DEBUG_MSG("tasklist window");

  currentRow = row;

  tasklist_window = window_create();
  window_set_window_handlers(tasklist_window, (WindowHandlers) {
    .load = tasklist_window_load,
    .unload = tasklist_window_unload,
  });
  //window_set_click_config_provider(tasklist_window, (ClickConfigProvider) tasklist_click_config);
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


  update_task_count();

  task_menu_layer = menu_layer_create(bounds);

  menu_layer_set_highlight_colors(task_menu_layer, GColorWhite, GColorBlack);

  menu_layer_set_click_config_onto_window(task_menu_layer, window);
  menu_layer_set_callbacks(task_menu_layer, NULL, (MenuLayerCallbacks) {
    .get_num_rows = get_num_rows_callback,
    .draw_row = draw_row_callback,
    .get_cell_height = get_cell_height_callback,
    .select_click = select_callback,
    //.select_long_click = long_callback,
  });

  layer_add_child(window_layer, menu_layer_get_layer(task_menu_layer));
  layer_add_child(window_layer, s_header_layer);


  /*for(int i = 0; i<menuRows[currentRow].tasks; i++){
  //for(int i = 0; i<2; i++){
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

}*/

}

static uint16_t get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data){
  return menuRows[currentRow].tasks;
}


static void draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data){
  GRect bounds = layer_get_bounds(cell_layer);

  GRect card_bounds = GRect(
    bounds.origin.x + CARD_OFFSET_W,
    bounds.origin.y + CARD_OFFSET_H,
    bounds.size.w - (2 * CARD_OFFSET_W),
    CARD_HEIGHT
  );

  GRect top_bounds =  GRect(card_bounds.origin.x+2, card_bounds.origin.y+2, card_bounds.size.w-4, card_bounds.size.h-4);



  #ifdef PBL_COLOR
  // Set the fill color
  graphics_context_set_fill_color(ctx, GColorLightGray);
  #else
  // Set the fill color
  graphics_context_set_fill_color(ctx, GColorBlack);
  #endif

  graphics_fill_rect(ctx, card_bounds, 0, GCornersAll);

  graphics_context_set_fill_color(ctx, GColorWhite);

  graphics_fill_rect(ctx, top_bounds, 0, GCornersAll);

  // Set the font color
  graphics_context_set_text_color(ctx, GColorBlack);

  GFont font = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);

  DEBUG_MSG("before task pointer");

  Task *task;
  task = linked_list_get(menuRows[currentRow].taskList, (int)cell_index->row);

  DEBUG_MSG("after task pointer");
  DEBUG_MSG("crash time: %d", task->time);
  //DEBUG_MSG("crash text: %s", task->description);

  char *card_text;
  if(task->description!=NULL){
    card_text = malloc(sizeof(char) * (strlen(task->description) + 1));
    strcpy(card_text, task->description);
  }else{
    card_text = malloc(sizeof(char) * (strlen("") + 1));
    strcpy(card_text, "");
  }

  DEBUG_MSG("after text assign");


  // Determine a reduced bounding box
  GRect text_bounds = GRect(top_bounds.origin.x, top_bounds.origin.y + (CARD_HEIGHT/4),
  card_bounds.size.w, (CARD_HEIGHT/2));


  // Draw the text
  graphics_draw_text(ctx, card_text, font, text_bounds, GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);

}



static void select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data){
  tasklist_button_select((int)cell_index->row);
}
static int16_t get_cell_height_callback(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *context){
  return CARD_HEIGHT + CARD_OFFSET_H;
}

static void update_task_count() {
  tasksLength = get_int_length(menuRows[currentRow].tasks);

  DEBUG_MSG("Int length: %d", tasksLength);

  char *number_buffer = malloc(sizeof(char) *  (tasksLength + 1));
  dec_to_str(number_buffer, menuRows[currentRow].tasks, tasksLength);

  DEBUG_MSG("number_buffer: %s", number_buffer);

  text = malloc(sizeof(char) * (strlen("Tasks: ") + tasksLength + 1));
  //text = "Tasks: ";
  strcpy(text, "Tasks: ");

  DEBUG_MSG("mid text: %s", text);

  strcat(text, number_buffer);

  free(number_buffer);
  number_buffer = NULL;

  DEBUG_MSG("Final text: %s", text);


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
  graphics_draw_text(ctx, text, font, bounds, GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);



}




static void tasklist_button_select(int task_index) {

  if(menuRows[currentRow].tasks>0){

    DictionaryIterator *iter;

    DEBUG_MSG("iter created");

    if(app_message_outbox_begin(&iter) == APP_MSG_OK) {

      DEBUG_MSG("ok appmessage");


      dict_write_uint8(iter, MESSAGE_KEY_TableRemoveRow, (uint8_t)currentRow);

      DEBUG_MSG("index");

      //task_index = 0;

      dict_write_uint8(iter, MESSAGE_KEY_TableRemoveIndex, (uint8_t)task_index);

      DEBUG_MSG("written");



      DEBUG_MSG("debug");

      if(!comm_is_js_ready()){
        return;
      }

      // Send this message
      if(app_message_outbox_send() != APP_MSG_OK) {
        DEBUG_MSG("Error sending the outbox");
      }else{
        DEBUG_MSG("sent");
        //window_stack_pop(add_task_window);
        //free(task_text);

        setMenuCount(menuRows[currentRow].tasks - 1, currentRow);
        update_task_count();
        //layer_mark_dirty(task_card_placeholder);

        DEBUG_MSG("free");

      }
    } else {
      // The outbox cannot be used right now
      DEBUG_MSG("Error preparing the outbox");
    }
  }
}

int get_int_length(unsigned x) {
  if (x >= 100)        return 3;
  if (x >= 10)         return 2;
  return 1;
}
