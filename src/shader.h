#pragma once

#include "common.h"

typedef u32 shader_id;

typedef enum shader_kind {
  SHADER_KIND_VERT = 1,
  SHADER_KIND_FRAG,
} shader_kind;

typedef struct shader {
  shader_id program;
} shader;

b32 compile_shader_part(shader_id *out, cstr src, u64 len, shader_kind kind);
b32 compile_shader(shader *s, shader_id vert, shader_id frag);

b32 create_shader(shader *s, cstr vert_path, cstr frag_path);
b32 destroy_shader(shader *s);
