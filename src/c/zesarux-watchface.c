//https://developer.rebble.io/tutorials/watchface-tutorial/part1/

#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;
static BitmapLayer *s_background_layer;

static GBitmap *s_background_bitmap0;
static GBitmap *s_background_bitmap1;
static GBitmap *s_background_bitmap2;
static GBitmap *s_background_bitmap3;
static GBitmap *s_background_bitmap4;

unsigned int id_background=0;

// Persistent storage key
#define SETTINGS_KEY 1

// Define our settings struct
typedef struct ClaySettings {
  bool Animations;
} ClaySettings;

// An instance of the struct
static ClaySettings settings;

// Save the settings to persistent storage
static void prv_save_settings() {
  persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
}

// Initialize the default settings
static void prv_default_settings() {
  settings.Animations = true;
}

// Read settings from persistent storage
static void prv_load_settings() {
  // Load the default settings
  prv_default_settings();
  // Read settings from persistent storage, if they exist
  persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
}

static void set_background() {


  int chosen_background = id_background % 5;

  // Set full image always if no animations set
  if (settings.Animations==0) chosen_background=4;

  switch(chosen_background) {
	  case 1:
		  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap1);
	  break;

	  case 2:
		  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap2);
	  break;

	  case 3:
		  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap3);
	  break;

	  case 4:
		  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap4);
	  break;

	  default:
		  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap0);
	   break;
  	}

}


// AppMessage receive handler
static void prv_inbox_received_handler(DictionaryIterator *iter, void *context) {
  // Assign the values to our struct
  Tuple *animations_t = dict_find(iter, MESSAGE_KEY_Animations);
  if(animations_t) {
    settings.Animations = animations_t->value->int32 == 1;
  }

  // ...
  prv_save_settings();
  set_background();
}


//Clay - config settings
void prv_init(void) {
  prv_load_settings();

  // Open AppMessage connection
  app_message_register_inbox_received(prv_inbox_received_handler);
  app_message_open(128, 128);

  // ...
}


static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
                                          "%H:%M" : "%I:%M", tick_time);

  id_background = tick_time->tm_min;

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, s_buffer);

  set_background();
}

static void main_window_load(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // Create GBitmap
    int bw=0;

  //Como se compila una versión para cada modelo, en la compilación se tienen estos macros
  //Nota: se probó con watch_info_get_model() pero no parecia funcionar
  #if defined(PBL_COLOR)
  bw=0;
  #else
  bw=1;
  #endif

  if (bw) {
  s_background_bitmap0 = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BW_BACKGROUND0);
  s_background_bitmap1 = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BW_BACKGROUND1);
  s_background_bitmap2 = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BW_BACKGROUND2);
  s_background_bitmap3 = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BW_BACKGROUND3);
  s_background_bitmap4 = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BW_BACKGROUND4);
  }
  else {
  s_background_bitmap0 = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND0);
  s_background_bitmap1 = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND1);
  s_background_bitmap2 = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND2);
  s_background_bitmap3 = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND3);
  s_background_bitmap4 = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND4);
  }

  // Create BitmapLayer to display the GBitmap
  s_background_layer = bitmap_layer_create(bounds);

  // Set the bitmap onto the layer and add to the window
  //bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  set_background();

  layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));

  // Create the TextLayer with specific bounds
  s_time_layer = text_layer_create(
      GRect(0, 0 /*PBL_IF_ROUND_ELSE(58, 52)*/, bounds.size.w, 50));

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlue);
  text_layer_set_text(s_time_layer, "00:00");
  //text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);

  // Destroy BitmapLayer
  bitmap_layer_destroy(s_background_layer);

  // Destroy GBitmap
  gbitmap_destroy(s_background_bitmap0);
  gbitmap_destroy(s_background_bitmap1);
  gbitmap_destroy(s_background_bitmap2);
  gbitmap_destroy(s_background_bitmap3);
  gbitmap_destroy(s_background_bitmap4);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void init() {
  prv_init();

  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Change the background color
  //window_set_background_color(s_main_window, GColorBlack);

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);

  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  // Make sure the time is displayed from the start
  update_time();
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}



