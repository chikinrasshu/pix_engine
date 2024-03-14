#pragma once

#include "bitmap.h"
#include "common.h"
#include "shader.h"
#include "vec.h"

typedef struct gl_window gl_window;
#define GL_WINDOW_RUN_FN(Name) void Name(gl_window *win)
typedef GL_WINDOW_RUN_FN(gl_window_run_fn);

typedef struct vertex {
  v3 pos;
  v2 uv;
} vertex;

typedef struct gl_window_renderer {
  shader sh;
  u32 vao, vbo, ebo, tex;

  vertex vertices[4];
  u32 indices[6];
} gl_window_renderer;

typedef struct gl_window {
  b32 running;
  b32 locked_internal_res;
  b32 integer_scaling;

  v2i pos;
  v2i size;
  v2 scale;
  v4 viewport;

  v2 mouse_pos, raw_mouse_pos;

  r64 dt, ct, lt;

  u64 frame_count;
  bitmap fb;
  gl_window_renderer gl;

  gl_window_run_fn *run_fn;

  void *_ptr;
} gl_window;

b32 create_gl_window(gl_window *win, v2i size, cstr caption);
b32 destroy_gl_window(gl_window *win);

b32 step_gl_window(gl_window *win, b32 process_events);
b32 run_gl_window(gl_window *win);

b32 lock_gl_window_internal_resolution(gl_window *win, v2i size);
b32 unlock_gl_window_internal_resolution(gl_window *win);
