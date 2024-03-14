#include "bitmap.h"
#include "color.h"
#include "gl_window.h"
#include <stdlib.h>

void draw_pixel(bitmap *bmp, s32 x, s32 y, u32 color) {
  if (x < 0 || x >= bmp->size.x)
    return;
  if (y < 0 || y >= bmp->size.y)
    return;

  u8 *ptr = bmp->memory + bmp->stride * y;
  u32 *pixel = (u32 *)ptr + x;
  *pixel = color;
}

void draw_vline(bitmap *bmp, r32 x, r32 y0, r32 y1, v4 color) {
  s32 ax = (s32)x;
  s32 ay = (s32)y0;
  s32 by = (s32)y1;

  if (ay > by) {
    s32 tmp = ay;
    ay = by;
    by = tmp;
  }

  u32 packed = pack_rgba((u8)(color.x * 255.0f), (u8)(color.y * 255.0f),
                         (u8)(color.z * 255.0f), (u8)(color.w * 255.0f));

  for (s32 y = ay; y < by; ++y) {
    draw_pixel(bmp, ax, y, packed);
  }
}

void draw_hline(bitmap *bmp, r32 y, r32 x0, r32 x1, v4 color) {
  s32 ax = (s32)x0;
  s32 ay = (s32)y;
  s32 bx = (s32)x1;

  if (ax > bx) {
    s32 tmp = ax;
    ax = bx;
    bx = tmp;
  }

  u32 packed = pack_rgba((u8)(color.x * 255.0f), (u8)(color.y * 255.0f),
                         (u8)(color.z * 255.0f), (u8)(color.w * 255.0f));

  for (s32 x = ax; x < bx; ++x) {
    draw_pixel(bmp, x, ay, packed);
  }
}

void draw_line(bitmap *bmp, v2 a, v2 b, v4 color) {
  // Axis aligned lines
  if (a.x == b.x) {
    draw_vline(bmp, a.x, a.y, b.y, color);
    return;
  }

  if (a.y == b.y) {
    draw_hline(bmp, a.y, a.x, b.x, color);
    return;
  }

  u32 packed = pack_rgba((u8)(color.x * 255.0f), (u8)(color.y * 255.0f),
                         (u8)(color.z * 255.0f), (u8)(color.w * 255.0f));

  // Generalized lines
  s32 x0 = (s32)a.x, y0 = (s32)a.y, x1 = (s32)b.x, y1 = (s32)b.y;

  int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
  int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
  int err = dx + dy, e2;

  for (;;) {
    draw_pixel(bmp, x0, y0, packed);
    if (x0 == x1 && y0 == y1)
      break;

    e2 = 2 * err;
    if (e2 >= dy)
      err += dy, x0 += sx;

    if (e2 <= dx)
      err += dx, y0 += sy;
  }
}

void on_frame(gl_window *win) {
  v2 mp = win->mouse_pos;

  clear_bitmap(&win->fb, pack_rgba(16, 32, 64, 255));
  draw_line(&win->fb, (v2){4, 6}, mp, (v4){0.4f, 0.5f, 0.6f, 1.0f});
}

int main(void) {
  gl_window win = {0};
  if (!create_gl_window(&win, (v2i){800, 600}, "pix_engine")) {
    return 1;
  }
  win.run_fn = on_frame;

  win.integer_scaling = true;
  lock_gl_window_internal_resolution(&win, (v2i){100, 75});

  run_gl_window(&win);
  destroy_gl_window(&win);
  return 1;
}
