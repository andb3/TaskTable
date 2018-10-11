static Window *loading_window;


void loading_window_load(Window *window) {

}

void loading_window_unload(Window *window) {

}

void loading_init(void) {


  loading_window = window_create();
  window_set_window_handlers(loading_window, (WindowHandlers) {
    .load = loading_window_load,
    .unload = loading_window_unload,
  });
  const bool animated = true;
  window_stack_push(loading_window, animated);
}

void loading_deinit(void) {
  window_destroy(loading_window);
}
