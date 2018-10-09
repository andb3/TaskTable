#include <pebble.h>
#include "AppMessages.h"

static void in_received_handler(DictionaryIterator *iter, void *context);
static void in_dropped_handler(AppMessageResult reason, void *context);
static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context);
//static void outbox_sent_handler(DictionaryIterator *iter, void *context);


static void in_received_handler(DictionaryIterator *iter, void *context){

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
