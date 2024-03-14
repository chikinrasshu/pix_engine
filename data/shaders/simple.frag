#version 410

in vec2 vert_uv;

out vec4 out_col;

uniform sampler2D tex;

void main() {
  out_col = texture(tex, vert_uv);
}
