#include "gl_window.h"
#include "bitmap.h"
#include "common.h"
#include "shader.h"

#include <glad/gl.h>

#include <GLFW/glfw3.h>

#include <math.h>
#include <stddef.h>
#include <stdio.h>

static s32 g_gl_window_count = 0;

b32 push_gl_window(void) {
  if (g_gl_window_count == 0) {
    if (!glfwInit()) {
      return false;
    } else {
      printf("Initialized GLFW\n");
    }
  }

  ++g_gl_window_count;
  return true;
}

b32 pop_gl_window(void) {
  if (g_gl_window_count == 1)
    glfwTerminate();
  --g_gl_window_count;
  return true;
}

// fwd callbacks
void gl_window_cb_close(GLFWwindow *ptr);
void gl_window_cb_refresh(GLFWwindow *ptr);
void gl_window_cb_pos(GLFWwindow *ptr, s32 x, s32 y);
void gl_window_cb_size(GLFWwindow *ptr, s32 w, s32 h);
void gl_window_cb_size_fb(GLFWwindow *ptr, s32 fw, s32 fh);
void gl_window_cb_scale(GLFWwindow *ptr, r32 sx, r32 sy);
void gl_window_cb_mouse_pos(GLFWwindow *ptr, r64 mx, r64 my);

b32 init_gl_window_callbacks(gl_window *win) {
  if (!win) {
    printf("win was NULL\n");
    return false;
  }

  glfwSetWindowUserPointer(win->_ptr, win);
  glfwSetWindowCloseCallback(win->_ptr, gl_window_cb_close);
  glfwSetWindowRefreshCallback(win->_ptr, gl_window_cb_refresh);
  glfwSetWindowPosCallback(win->_ptr, gl_window_cb_pos);
  glfwSetWindowSizeCallback(win->_ptr, gl_window_cb_size);
  glfwSetFramebufferSizeCallback(win->_ptr, gl_window_cb_size_fb);
  glfwSetWindowContentScaleCallback(win->_ptr, gl_window_cb_scale);
  glfwSetCursorPosCallback(win->_ptr, gl_window_cb_mouse_pos);

  return true;
}

b32 init_gl_window_defaults(gl_window *win) {
  if (!win) {
    printf("win was NULL\n");
    return false;
  }

  glfwGetWindowSize(win->_ptr, &win->size.x, &win->size.y);
  glfwGetWindowContentScale(win->_ptr, &win->scale.x, &win->scale.y);
  glfwGetFramebufferSize(win->_ptr, &win->fb.size.x, &win->fb.size.y);

  r64 a, b;
  glfwGetCursorPos(win->_ptr, &a, &b);
  win->raw_mouse_pos.x = (r32)a, win->raw_mouse_pos.y = (r32)b;

  if (!create_bitmap(&win->fb, win->fb.size, 4)) {
    printf("Failed to create the main framebuffer of size %dx%d\n",
           win->fb.size.x, win->fb.size.y);
    return false;
  }
  win->viewport = (v4){0, 0, win->fb.size.x, win->fb.size.y};

  win->ct = win->lt = glfwGetTime();
  win->dt = 1;

  return true;
}

b32 init_gl_window_opengl(gl_window *win) {
  if (!win) {
    printf("win was NULL\n");
    return false;
  }

  glfwMakeContextCurrent(win->_ptr);
  s32 version = gladLoadGL(glfwGetProcAddress);
  if (!version) {
    printf("Failed to initialize OpenGL\n");
    return false;
  }
  printf("Loaded OpenGL v%d.%d\n", GLAD_VERSION_MAJOR(version),
         GLAD_VERSION_MINOR(version));
  glfwSwapInterval(1);

  if (!create_shader(&win->gl.sh, "data/shaders/simple.vert",
                     "data/shaders/simple.frag")) {
    return false;
  }

  r32 s = 1;

  // {{-s, -s, 0}, {+s, -s, 0}, {+s, +s, 0}, {-s, +s, 0}};
  win->gl.vertices[0] = (vertex){{+s, +s, 0}, {1, 0}};
  win->gl.vertices[1] = (vertex){{+s, -s, 0}, {1, 1}};
  win->gl.vertices[2] = (vertex){{-s, -s, 0}, {0, 1}};
  win->gl.vertices[3] = (vertex){{-s, +s, 0}, {0, 0}};

  // {0, 2, 3, 2, 3, 0}
  win->gl.indices[0] = 0;
  win->gl.indices[1] = 1;
  win->gl.indices[2] = 3;
  win->gl.indices[3] = 1;
  win->gl.indices[4] = 2;
  win->gl.indices[5] = 3;

  glGenTextures(1, &win->gl.tex);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, win->gl.tex);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, win->fb.size.x, win->fb.size.y, 0,
               GL_RGBA, GL_UNSIGNED_BYTE, win->fb.memory);
  glGenerateMipmap(GL_TEXTURE_2D);

  glGenVertexArrays(1, &win->gl.vao);
  glGenBuffers(1, &win->gl.vbo);
  glGenBuffers(1, &win->gl.ebo);

  glBindVertexArray(win->gl.vao);
  // Upload the vertices to the GPU
  glBindBuffer(GL_ARRAY_BUFFER, win->gl.vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(win->gl.vertices), win->gl.vertices,
               GL_STATIC_DRAW);
  // Upload the indices to the GPU
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, win->gl.ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(win->gl.indices),
               win->gl.indices, GL_STATIC_DRAW);
  // Define 3d vertex with layout [pos]
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex),
                        (void *)offsetof(vertex, pos));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex),
                        (void *)offsetof(vertex, uv));
  glEnableVertexAttribArray(1);

  return true;
}

b32 create_gl_window(gl_window *win, v2i size, cstr caption) {
  if (!push_gl_window()) {
    return false;
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GLFW_OPENGL_FORWARD_COMPAT);

  win->_ptr = glfwCreateWindow(size.x, size.y, caption, NULL, NULL);
  if (!win->_ptr) {
    printf("Failed to create the main window named '%s' of size %dx%d\n",
           caption, size.x, size.y);
    return false;
  }

  if (!init_gl_window_opengl(win)) {
    return false;
  }

  if (!init_gl_window_callbacks(win)) {
    return false;
  }

  if (!init_gl_window_defaults(win)) {
    return false;
  }

  win->running = true;
  return win->_ptr != NULL;
}

b32 destroy_gl_window(gl_window *win) {
  if (!win) {
    printf("win was NULL\n");
    return false;
  }

  destroy_shader(&win->gl.sh);

  if (win->_ptr) {
    glfwDestroyWindow(win->_ptr);
    win->_ptr = NULL;
  }

  return pop_gl_window();
}

r32 map_1d(r32 v, r32 src_a, r32 src_b, r32 dst_a, r32 dst_b) {
  r32 slope = 1.0 * (dst_b - dst_a) / (src_b - src_a);
  return dst_a + slope * (v - src_a);
}

b32 step_gl_window(gl_window *win, b32 process_events) {
  if (!win) {
    printf("win was NULL\n");
    return false;
  }
  if (!win->_ptr) {
    printf("win->_ptr was NULL\n");
    return false;
  }

  if (win->running) {
    if (process_events) {
      glfwPollEvents();
    }

    // Process the frame
    if (win->run_fn) {
      win->run_fn(win);
      ++win->frame_count;
    }

    // Upload the framebuffer (assume it changes every frame)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, win->fb.size.x, win->fb.size.y, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, win->fb.memory);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Calculate the scale from internal resolution to window rectangle in
    // pixels
    s32 sx = 0, sy = 0, sw = win->fb.size.x, sh = win->fb.size.y;
    r32 scale = 1.0f;

    s32 sww, swh;
    glfwGetFramebufferSize(win->_ptr, &sww, &swh);
    if (win->locked_internal_res) {

      r32 ww = (r32)sww, wh = (r32)swh;
      r32 fw = (r32)win->fb.size.x, fh = (r32)win->fb.size.y;
      scale = fminf(ww / fw, wh / fh);
      if (scale >= 1.0 && win->integer_scaling) {
        scale = floorf(scale);
      }

      sw = (s32)(fw * scale), sh = (s32)(fh * scale);
      sx = (sww - sw) / 2, sy = (swh - sh) / 2;
    }

    r32 mx = win->raw_mouse_pos.x * win->scale.x,
        my = win->raw_mouse_pos.y * win->scale.y;

    win->viewport = (v4){sx, sy, sw, sh};
    win->mouse_pos.x = floorf(map_1d(mx, sx, sx + sw, 0, win->fb.size.x));
    win->mouse_pos.y = floorf(map_1d(my, sy, sy + sh, 0, win->fb.size.y));

    printf("rmp: %.2fx%.2f; mp: %.2fx%.2f; fb: %dx%d; raw: %dx%d\n",
           win->raw_mouse_pos.x, win->raw_mouse_pos.y, win->mouse_pos.x,
           win->mouse_pos.y, win->fb.size.x, win->fb.size.y, sww, swh);

    // Render the frame
    glViewport(0, 0, sww, swh);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(win->viewport.x, win->viewport.y, win->viewport.z,
               win->viewport.w);

    glUseProgram(win->gl.sh.program);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, win->gl.tex);
    glUniform1i(glGetUniformLocation(win->gl.sh.program, "tex"), 0);

    glBindVertexArray(win->gl.vao);
    glDrawElements(GL_TRIANGLES, array_count(win->gl.indices), GL_UNSIGNED_INT,
                   0);

    // Present the frame
    glfwSwapBuffers(win->_ptr);
    glFinish();

    // Calculate delta time
    win->lt = win->ct;
    win->ct = glfwGetTime();
    win->dt = win->ct - win->lt;
  }
  return win->running;
}

b32 run_gl_window(gl_window *win) {
  if (!win) {
    printf("win was NULL\n");
    return false;
  }
  if (!win->_ptr) {
    printf("win->_ptr was NULL\n");
    return false;
  }

  while (win->running) {
    step_gl_window(win, true);
  }
  return true;
}

b32 lock_gl_window_internal_resolution(gl_window *win, v2i size) {
  if (!win) {
    printf("win was NULL\n");
    return false;
  }

  win->locked_internal_res = true;
  win->fb.size = size;
  return resize_bitmap(&win->fb, win->fb.size);
}

b32 unlock_gl_window_internal_resolution(gl_window *win) {
  if (!win) {
    printf("win was NULL\n");
    return false;
  }

  if (win->locked_internal_res) {
    win->locked_internal_res = false;

    glfwGetFramebufferSize(win->_ptr, &win->fb.size.x, &win->fb.size.y);
    return resize_bitmap(&win->fb, win->fb.size);
  }

  return true;
}

// imp callbacks
void gl_window_cb_close(GLFWwindow *ptr) {
  gl_window *win = glfwGetWindowUserPointer(ptr);
  if (win) {
    win->running = false;
  }
}

void gl_window_cb_refresh(GLFWwindow *ptr) {
  gl_window *win = glfwGetWindowUserPointer(ptr);
  if (win) {
    step_gl_window(win, false);
  }
}

void gl_window_cb_pos(GLFWwindow *ptr, s32 x, s32 y) {
  gl_window *win = glfwGetWindowUserPointer(ptr);
  if (win) {
    win->pos.x = x;
    win->pos.y = y;
  }
}

void gl_window_cb_size(GLFWwindow *ptr, s32 w, s32 h) {
  gl_window *win = glfwGetWindowUserPointer(ptr);
  if (win) {
    win->size.x = w;
    win->size.y = h;
    printf("Resized window to %dx%d\n", w, h);
  }
}

void gl_window_cb_size_fb(GLFWwindow *ptr, s32 fw, s32 fh) {
  gl_window *win = glfwGetWindowUserPointer(ptr);
  if (win && !win->locked_internal_res) {
    resize_bitmap(&win->fb, (v2i){fw, fh});
    win->viewport = (v4){0, 0, fw, fh};
  }
}

void gl_window_cb_scale(GLFWwindow *ptr, r32 sx, r32 sy) {
  gl_window *win = glfwGetWindowUserPointer(ptr);
  if (win) {
    win->scale.x = sx;
    win->scale.y = sy;
  }
}

void gl_window_cb_mouse_pos(GLFWwindow *ptr, r64 mx, r64 my) {
  gl_window *win = glfwGetWindowUserPointer(ptr);
  if (win) {
    win->raw_mouse_pos.x = (r32)mx;
    win->raw_mouse_pos.y = (r32)my;
  }
}
