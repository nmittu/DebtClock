#include <pebble.h>

#define KEY_DEBT 0
#define KEY_IS_GREEN 1

Window *window;
TextLayer *t_layer;
char *debt;
static void inbox_received_callback(DictionaryIterator *iterator, void *context){
	free(debt);
	debt = (char*) malloc(sizeof(char) *30);
	strcpy(debt, dict_find(iterator, KEY_DEBT)->value->cstring);
	
	if(dict_find(iterator, KEY_IS_GREEN)->value->int8){
		text_layer_set_text_color(t_layer, GColorGreen);
	}else{
		text_layer_set_text_color(t_layer, GColorRed);
	}
	
	layer_mark_dirty(text_layer_get_layer(t_layer));
}

static void inbox_dropped_callback(AppMessageResult reason, void *context){
	APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context){
	APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context){
	APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send success!!");
}

void window_load(Window *window){
	Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
	
	t_layer = text_layer_create(GRect(0, PBL_IF_ROUND_ELSE(60, 54), bounds.size.w, 100));
	
	text_layer_set_background_color(t_layer, GColorClear);
  text_layer_set_text_color(t_layer, GColorBlack);
	text_layer_set_text_alignment(t_layer, GTextAlignmentCenter);
	text_layer_set_overflow_mode(t_layer, GTextOverflowModeFill);
  text_layer_set_text(t_layer, debt);
  text_layer_set_font(t_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
	
	layer_add_child(window_layer, text_layer_get_layer(t_layer));
}

void window_unload(Window *window){
	text_layer_destroy(t_layer);
}

void init(){
	debt = (char*) malloc(sizeof(char) *30);
	strcpy(debt, "Loading...");
	
	window = window_create();
	window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload
  });
	
	window_stack_push(window, true);
	
	app_message_register_inbox_received(inbox_received_callback);
	app_message_register_inbox_dropped(inbox_dropped_callback);
	app_message_register_outbox_failed(outbox_failed_callback);
	app_message_register_outbox_sent(outbox_sent_callback);
	
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}

void deinit(){
	window_destroy(window);
	free(debt);
}

int main(){
	init();
	app_event_loop();
	deinit();
}