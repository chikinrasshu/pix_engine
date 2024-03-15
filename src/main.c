#include "bitmap.h"
#include "color.h"
#include "gl_window.h"
#include "soft.h"

void on_frame(gl_window *win) {
  v2 mp = win->mouse_pos;

  clear_bitmap(&win->fb, pack_rgba(16, 32, 64, 255));
  draw_triangle(&win->fb, (v2){33, 18}, (v2){5, 7}, mp,
                (v4){0.4f, 0.5f, 0.6f, 1.0f});
}

int main(void) {
  gl_window win = {0};
  if (!create_gl_window(&win, (v2i){1280, 720}, "pix_engine")) {
    return 1;
  }
  win.run_fn = on_frame;

  win.integer_scaling = true;
  lock_gl_window_internal_resolution(&win, (v2i){100, 75});

  run_gl_window(&win);
  destroy_gl_window(&win);
  return 1;
}
