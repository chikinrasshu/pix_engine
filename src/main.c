#include "gl_window.h"

#include <stdio.h>

void on_frame(gl_window *win) {
  printf("\rRunning at %.2f fps\n", 1.0 / win->dt);
}

int main() {
  gl_window win = {0};
  if (!create_gl_window(&win, 800, 600, "pix_engine")) {
    return 1;
  }
  win.run_fn = on_frame;

  win.integer_scaling = true;
  lock_gl_window_internal_resolution(&win, 200, 150);

  run_gl_window(&win);
  destroy_gl_window(&win);
  return 1;
}
