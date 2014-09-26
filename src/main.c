#include <pebble.h>

#include "game.h"

static Window *window;
static Layer *render_layer;

static void register_timer(void* data);

//#define FPS 1

#define MAX(A,B) ((A>B) ? A : B)
#define MIN(A,B) ((A<B) ? A : B)

extern uint8_t *screen_buffer;

static void register_timer(void* data);

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  move_left();
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  move_right();
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

#ifdef FPS
//Currently about 3fps for filled, 4fps for wireframe
static uint16_t frame_count = 0;
#endif

static void render_opengl(Layer* layer, GContext *ctx) {
  GBitmap* bitmap = (GBitmap*)ctx;
  screen_buffer = (uint8_t*)bitmap->addr;

  gl_drawframe();
#ifdef FPS
  frame_count++;
#endif
  app_timer_register(10, register_timer, NULL);
}

static void register_timer(void* data) {
  layer_mark_dirty(render_layer);
}

#ifdef FPS
static void register_fps_timer(void* data) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "5s frame_count %d", frame_count);
  frame_count = 0;
  app_timer_register(5000, register_fps_timer, data);
}
#endif

static void window_load(Window *window) {
  Layer* window_layer = window_get_root_layer(window);
  render_layer = layer_create(layer_get_bounds(window_layer));
  layer_set_update_proc(render_layer, render_opengl);
  layer_add_child(window_layer, render_layer);
  register_timer(NULL);
#ifdef FPS
  register_fps_timer(NULL);
#endif
}

static void window_unload(Window *window) {}

static void init(void) {
  light_enable(true);  // Leave the backlight on

  window = window_create();
  window_set_fullscreen(window, true);
  window_set_background_color(window, GColorBlack);
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(window, true);

  gl_init();
}

static void deinit(void) {
  window_destroy(window);
}

__attribute__((used))
extern int main(void) {
  init();

  app_event_loop();
  deinit();
  return 0;
}
