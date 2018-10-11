#include <pebble.h>
#include "AddTask.h"
#include "TaskTable.h"
#include "AppMessages.h"


#define NUM_ITEMS 8

static int s_index;
static char *table_data[NUM_ITEMS];
static bool table_loaded = false;

static bool s_js_ready;


static void in_received_handler(DictionaryIterator *iter, void *context);
static void in_dropped_handler(AppMessageResult reason, void *context);
static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context);
//static void outbox_sent_handler(DictionaryIterator *iter, void *context);


static void in_received_handler(DictionaryIterator *iter, void *context){
	DEBUG_MSG("Recieved Message");
	Tuple *ready_tuple = dict_find(iter, MESSAGE_KEY_JSReady);
	if(ready_tuple) {
		DEBUG_MSG("JS Ready");
		// PebbleKit JS is ready! Safe to send messages
		s_js_ready = true;
	}

	Tuple *table_data_tuple = dict_find(iter, MESSAGE_KEY_TableUpdate + s_index);
  if(table_data_tuple) {
    // Store this item
    table_data[s_index] = table_data_tuple->value->cstring;

    // Increment index for next item
    s_index++;
  }

  if(s_index == NUM_ITEMS) {
    // We have reached the end of the sequence
    APP_LOG(APP_LOG_LEVEL_INFO, "All transmission complete!");
		table_init();
  }

}

static void in_dropped_handler(AppMessageResult reason, void *context){

}

static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context){

}

/*static void outbox_sent_handler(DictionaryIterator *iter, void *context) {

}*/

void app_message_init()
{
	app_message_register_inbox_received(in_received_handler);
	app_message_register_inbox_dropped(in_dropped_handler);
	app_message_register_outbox_failed(out_failed_handler);

	int max = app_message_inbox_size_maximum();

	int inboxSize = max > 1024 ? 1024 : max;

	app_message_open(inboxSize, 256);
}

bool comm_is_js_ready() {
  return s_js_ready;
}
