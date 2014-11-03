#include "face.h"
#include <pebble.h>
  
  // http://forums.getpebble.com/discussion/10690/the-address-of-will-always-evaluate-as-true
#define MyTupletCString(_key, _cstring) \
((const Tuplet) { .type = TUPLE_CSTRING, .key = _key, .cstring = { .data = _cstring, .length = strlen(_cstring) + 1 }})
  
// enigma address
static char adr[15] = "192.168.100.2";

// Key values for AppMessage Dictionary
enum {
  // keys for data from enigma - incoming
	BEGIN_KEY = 0,
  STATION_KEY = 1,
  NAME_KEY = 2,
  END_KEY = 3,
  DESCRIPTION_KEY = 4,
  // keys for data from settings - incoming
  ADDRESS_KEY = 5,
  REFRESH_KEY = 6,
  // key for data for ip request - outgoing
  IP_KEY = 7
};

// Key values for Settings
enum {
  ADDRESS_SETTING = 0,
  REFRESH_SETTING = 1
};

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_gothic_18_bold;
static GFont s_res_gothic_14;
static TextLayer *channel_textlayer;
static TextLayer *programme_textlayer;
static InverterLayer *s_inverterlayer_1;
static InverterLayer *s_inverterlayer_2;
static TextLayer *begin_textlayer;
static TextLayer *minus_textlayer;
static TextLayer *end_textlayer;
static TextLayer *description_textlayer;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_fullscreen(s_window, 1);
  
  s_res_gothic_18_bold = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  s_res_gothic_14 = fonts_get_system_font(FONT_KEY_GOTHIC_14);
  // channel_textlayer
  channel_textlayer = text_layer_create(GRect(3, 3, 138, 24));
  text_layer_set_background_color(channel_textlayer, GColorBlack);
  text_layer_set_text_color(channel_textlayer, GColorWhite);
  text_layer_set_text(channel_textlayer, "EnigmaWatch");
  text_layer_set_text_alignment(channel_textlayer, GTextAlignmentCenter);
  text_layer_set_font(channel_textlayer, s_res_gothic_18_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)channel_textlayer);
  
  // programme_textlayer
  programme_textlayer = text_layer_create(GRect(3, 25, 138, 20));
  text_layer_set_text(programme_textlayer, "Loading...");
  text_layer_set_text_alignment(programme_textlayer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(s_window), (Layer *)programme_textlayer);
  
  // s_inverterlayer_1
  s_inverterlayer_1 = inverter_layer_create(GRect(20, 41, 104, 1));
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_inverterlayer_1);
  
  // s_inverterlayer_2
  s_inverterlayer_2 = inverter_layer_create(GRect(20, 152, 104, 1));
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_inverterlayer_2);
  
  // begin_textlayer
  begin_textlayer = text_layer_create(GRect(3, 153, 64, 20));
  text_layer_set_text(begin_textlayer, "00:00");
  text_layer_set_text_alignment(begin_textlayer, GTextAlignmentRight);
  layer_add_child(window_get_root_layer(s_window), (Layer *)begin_textlayer);
  
  // minus_textlayer
  minus_textlayer = text_layer_create(GRect(67, 153, 10, 20));
  text_layer_set_text(minus_textlayer, "-");
  text_layer_set_text_alignment(minus_textlayer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(s_window), (Layer *)minus_textlayer);
  
  // end_textlayer
  end_textlayer = text_layer_create(GRect(77, 153, 64, 20));
  text_layer_set_text(end_textlayer, "23:59");
  layer_add_child(window_get_root_layer(s_window), (Layer *)end_textlayer);
  
  // description_textlayer
  description_textlayer = text_layer_create(GRect(3, 42, 138, 110));
  text_layer_set_font(description_textlayer, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)description_textlayer);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(channel_textlayer);
  text_layer_destroy(programme_textlayer);
  inverter_layer_destroy(s_inverterlayer_1);
  inverter_layer_destroy(s_inverterlayer_2);
  text_layer_destroy(begin_textlayer);
  text_layer_destroy(minus_textlayer);
  text_layer_destroy(end_textlayer);
  text_layer_destroy(description_textlayer);
}
// END AUTO-GENERATED UI CODE

// send request for enigma data
void get_enigma_data(void){
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  Tuplet value = MyTupletCString(IP_KEY, adr);    
  dict_write_tuplet(iter, &value);
  //dict_write_end(iter);
  app_message_outbox_send();
}

static void handle_tick(struct tm *tick_time, TimeUnits units_changed) {
  get_enigma_data();
}

// Called when a message is received from PebbleKitJS
static void in_received_handler(DictionaryIterator *received, void *context) {
	Tuple *tuple;
	
  // when receiving enigma data following touples exist
	tuple = dict_find(received, STATION_KEY);
	if(tuple) {
		//APP_LOG(APP_LOG_LEVEL_DEBUG, "Received channel: %s", tuple->value->cstring); 
    text_layer_set_text(channel_textlayer, tuple->value->cstring);
	}
  
  tuple = dict_find(received, NAME_KEY);
	if(tuple) {
		//APP_LOG(APP_LOG_LEVEL_DEBUG, "Received programme: %s", tuple->value->cstring); 
    text_layer_set_text(programme_textlayer, tuple->value->cstring);
	}
  
  tuple = dict_find(received, DESCRIPTION_KEY);
	if(tuple) {
		//APP_LOG(APP_LOG_LEVEL_DEBUG, "Received desc: %s", tuple->value->cstring); 
    text_layer_set_text(description_textlayer, tuple->value->cstring);
	}
  
  tuple = dict_find(received, BEGIN_KEY);
	if(tuple) {
		//APP_LOG(APP_LOG_LEVEL_DEBUG, "Received begin: %s", tuple->value->cstring); 
    text_layer_set_text(begin_textlayer, tuple->value->cstring);
	}
  tuple = dict_find(received, END_KEY);
	if(tuple) {
		//APP_LOG(APP_LOG_LEVEL_DEBUG, "Received end: %s", tuple->value->cstring); 
    text_layer_set_text(end_textlayer, tuple->value->cstring);
	}
  
  // when receiving config data following two touples exist
  tuple = dict_find(received, ADDRESS_KEY);
	if(tuple) {
		//APP_LOG(APP_LOG_LEVEL_DEBUG, "Received address setting: %s", tuple->value->cstring); 
    persist_write_string(ADDRESS_SETTING, tuple->value->cstring);
    strncpy(adr,tuple->value->cstring,15);
    get_enigma_data();
	}
  
  tuple = dict_find(received, REFRESH_KEY);
	if(tuple) {
		//APP_LOG(APP_LOG_LEVEL_DEBUG, "Received refresh setting: %s", tuple->value->cstring); 
    if (strcmp(tuple->value->cstring,"y")==0) {
      persist_write_bool(REFRESH_SETTING,true);      
      tick_timer_service_subscribe(SECOND_UNIT, handle_tick);
    } else {
      persist_write_bool(REFRESH_SETTING,false);
      tick_timer_service_unsubscribe();
    }
	}
  
  // if javascript informs us that ip is unknown following is performed
  tuple = dict_find(received, IP_KEY);
	if(tuple) {
    get_enigma_data();
  }
}

// Called when an incoming message from PebbleKitJS is dropped
static void in_dropped_handler(AppMessageResult reason, void *context) {	
}

// Called when PebbleKitJS does not acknowledge receipt of a message
static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
}

// tapping refreshes data
static void handle_tap(AccelAxisType axis, int32_t direction) {
  get_enigma_data();
}

static void handle_window_unload(Window* window) {
  destroy_ui();
  tick_timer_service_unsubscribe();
  accel_tap_service_unsubscribe();
	app_message_deregister_callbacks();
}

void show_face(void) {
  if (persist_exists(ADDRESS_SETTING)){
      persist_read_string(ADDRESS_SETTING, adr, 15);
  }
  
  initialise_ui();
  // show ip for debugging
  text_layer_set_text(description_textlayer, adr);
  
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
  
  // Register AppMessage handlers
	app_message_register_inbox_received(in_received_handler); 
	app_message_register_inbox_dropped(in_dropped_handler); 
	app_message_register_outbox_failed(out_failed_handler);
		
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
	
  if (persist_exists(REFRESH_SETTING)){
    if (persist_read_bool(REFRESH_SETTING)) {
      //APP_LOG(APP_LOG_LEVEL_DEBUG, "Will set tick handler");
      tick_timer_service_subscribe(SECOND_UNIT, handle_tick);
    }
   }
  accel_tap_service_subscribe(handle_tap);
}

void hide_face(void) {
  window_stack_remove(s_window, true);
}