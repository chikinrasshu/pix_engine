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

  return true;
}

b32 init_gl_window_defaults(gl_window *win) {
  if (!win) {
    printf("win was NULL\n");
    return false;
  }

  glfwGetWindowSize(win->_ptr, &win->w, &win->h);
  glfwGetWindowContentScale(win->_ptr, &win->sx, &win->sy);
  glfwGetFramebufferSize(win->_ptr, &win->fb.w, &win->fb.h);

  if (!create_bitmap(&win->fb, win->fb.w, win->fb.h, 4)) {
    printf("Failed to create the main framebuffer of size %dx%d\n", win->fb.w,
           win->fb.h);
    return false;
  }

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
  win->gl.vertices[0] = (vertex){+s, +s, 0, 1, 1};
  win->gl.vertices[1] = (vertex){+s, -s, 0, 1, 0};
  win->gl.vertices[2] = (vertex){-s, -s, 0, 0, 0};
  win->gl.vertices[3] = (vertex){-s, +s, 0, 0, 1};

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

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, win->fb.w, win->fb.h, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, win->fb.memory);
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
                        (void *)offsetof(vertex, x));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex),
                        (void *)offsetof(vertex, u));
  glEnableVertexAttribArray(1);

  return true;
}

b32 create_gl_window(gl_window *win, s32 w, s32 h, cstr caption) {
  if (!push_gl_window()) {
    return false;
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GLFW_OPENGL_FORWARD_COMPAT);

  win->_ptr = glfwCreateWindow(w, h, caption, NULL, NULL);
  if (!win->_ptr) {
    printf("Failed to create the main window named '%s' of size %dx%d\n",
           caption, w, h);
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, win->fb.w, win->fb.h, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, win->fb.memory);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Calculate the scale from internal resolution to window rectangle in
    // pixels
    s32 sx = 0, sy = 0, sw = win->fb.w, sh = win->fb.h;
    glClear(GL_COLOR_BUFFER_BIT);

    if (win->locked_internal_res) {
      s32 sww, swh;
      glfwGetFramebufferSize(win->_ptr, &sww, &swh);

      r32 ww = (r32)sww, wh = (r32)swh;
      r32 fw = (r32)win->fb.w, fh = (r32)win->fb.h;
      r32 scale = fminf(ww / fw, wh / fh);
      if (scale >= 1.0 && win->integer_scaling) {
        scale = floorf(scale);
      }

      sw = (s32)(fw * scale), sh = (s32)(fh * scale);
      sx = (sww - sw) / 2, sy = (swh - sh) / 2;
    }
    // Render the frame
    glViewport(sx, sy, sw, sh);

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

b32 lock_gl_window_internal_resolution(gl_window *win, s32 w, s32 h) {
  if (!win) {
    printf("win was NULL\n");
    return false;
  }

  win->locked_internal_res = true;
  win->fb.w = w;
  win->fb.h = h;
  return resize_bitmap(&win->fb, win->fb.w, win->fb.h);
}

b32 unlock_gl_window_internal_resolution(gl_window *win) {
  if (!win) {
    printf("win was NULL\n");
    return false;
  }

  if (win->locked_internal_res) {
    win->locked_internal_res = false;

    glfwGetFramebufferSize(win->_ptr, &win->fb.w, &win->fb.h);
    return resize_bitmap(&win->fb, win->fb.w, win->fb.h);
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
    win->x = x;
    win->y = y;
  }
}

void gl_window_cb_size(GLFWwindow *ptr, s32 w, s32 h) {
  gl_window *win = glfwGetWindowUserPointer(ptr);
  if (win) {
    win->w = w;
    win->h = h;
    printf("Resized window to %dx%d\n", w, h);
  }
}

void gl_window_cb_size_fb(GLFWwindow *ptr, s32 fw, s32 fh) {
  gl_window *win = glfwGetWindowUserPointer(ptr);
  if (win && !win->locked_internal_res) {
    resize_bitmap(&win->fb, fw, fh);
  }
}

void gl_window_cb_scale(GLFWwindow *ptr, r32 sx, r32 sy) {
  gl_window *win = glfwGetWindowUserPointer(ptr);
  if (win) {
    win->sx = sx;
    win->sy = sy;
  }
}
