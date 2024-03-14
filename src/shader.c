#include "shader.h"
#include "file.h"

#include <glad/gl.h>

#include <stdio.h>
#include <stdlib.h>

b32 compile_shader_part(shader_id *out, cstr src, u64 len, shader_kind kind) {
  GLuint gl_kind = 0;
  switch (kind) {
  case SHADER_KIND_VERT:
    gl_kind = GL_VERTEX_SHADER;
    break;
  case SHADER_KIND_FRAG:
    gl_kind = GL_FRAGMENT_SHADER;
    break;
  }

  GLuint part = glCreateShader(gl_kind);
  if (!part) {
    printf("part was 0\n");
    return false;
  }

  const GLchar *const *code = (const GLchar *const *)&src;
  const GLint *lengths = (const GLint *)&len;

  glShaderSource(part, 1, code, lengths);
  glCompileShader(part);

  GLint success = 0;
  glGetShaderiv(part, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE) {
    GLint info_len = 0;
    glGetShaderiv(part, GL_INFO_LOG_LENGTH, &info_len);
    char *info_buffer = malloc(info_len);
    glGetShaderInfoLog(part, info_len, &info_len, info_buffer);

    printf("Compilation error: %s\n", info_buffer);

    glDeleteShader(part);
    part = 0;
  }

  *out = part;
  return part != 0;
}

b32 compile_shader(shader *s, shader_id vert, shader_id frag) {
  if (!s) {
    printf("s was NULL\n");
    return false;
  }

  s->program = glCreateProgram();
  if (!s->program) {
    printf("Failed to create the shader program\n");
    return false;
  }

  glAttachShader(s->program, vert);
  glAttachShader(s->program, frag);

  glLinkProgram(s->program);

  GLint success = 0;
  glGetProgramiv(s->program, GL_LINK_STATUS, &success);
  if (success == GL_FALSE) {
    GLint info_len = 0;
    glGetProgramiv(s->program, GL_INFO_LOG_LENGTH, &info_len);
    char *info_buffer = malloc(info_len);
    glGetProgramInfoLog(s->program, info_len, &info_len, info_buffer);

    printf("Linking error: %s\n", info_buffer);

    glDeleteProgram(s->program);
    s->program = 0;
  }

  glDetachShader(s->program, vert);
  glDetachShader(s->program, frag);

  return s->program != 0;
}

b32 create_shader(shader *s, cstr vert_path, cstr frag_path) {
  file vert_file = {0};
  if (!load_file(&vert_file, vert_path)) {
    return false;
  }

  file frag_file = {0};
  if (!load_file(&frag_file, frag_path)) {
    return false;
  }

  shader_id vert_id = 0;
  if (!compile_shader_part(&vert_id, (cstr)vert_file.memory, vert_file.size,
                           SHADER_KIND_VERT)) {
    return false;
  }

  shader_id frag_id = 0;
  if (!compile_shader_part(&frag_id, (cstr)frag_file.memory, frag_file.size,
                           SHADER_KIND_FRAG)) {
    return false;
  }

  b32 result = compile_shader(s, vert_id, frag_id);

  glDeleteShader(vert_id);
  glDeleteShader(frag_id);

  destroy_file(&vert_file);
  destroy_file(&frag_file);
  return result;
}

b32 destroy_shader(shader *s) {
  if (!s) {
    printf("s was NULL\n");
    return false;
  }

  if (s->program) {
    glDeleteProgram(s->program);
    s->program = 0;
  }

  return true;
}
