#include "soft.h"
#include "color.h"

#include <math.h>
#include <stdlib.h>

r32 dt_edge_fn(v2 a, v2 b, v2 c) {
  return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

void draw_triangle(bitmap *bmp, v2 a, v2 b, v2 c, v4 color) {
  // Get triangle bounds
  r32 min_x = fmaxf(fminf(fminf(a.x, b.x), c.x), 0);
  r32 min_y = fmaxf(fminf(fminf(a.y, b.y), c.y), 0);

  r32 max_x = fminf(fmaxf(fmaxf(a.x, b.x), c.x), bmp->size.x - 1);
  r32 max_y = fminf(fmaxf(fmaxf(a.y, b.y), c.y), bmp->size.y - 1);

  u8 *ptr = bmp->memory + bmp->stride * (s32)min_y + sizeof(u32) * (s32)min_x;

  u32 packed = pack_rgba((u8)(color.x * 255.0f), (u8)(color.y * 255.0f),
                         (u8)(color.z * 255.0f), (u8)(color.w * 255.0f));
  u32 packed_inv =
      pack_rgba(255 - (u8)(color.x * 255.0f), 255 - (u8)(color.y * 255.0f),
                255 - (u8)(color.z * 255.0f), (u8)(color.w * 255.0f));

  float area = dt_edge_fn(a, b, c);

  for (r32 y = min_y; y <= max_y; ++y) {
    u32 *pixel = (u32 *)ptr;
    for (r32 x = min_x; x <= max_x; ++x) {
      v2 p = {x + 0.5f, y + 0.5f};

      float w0 = dt_edge_fn(b, c, p);
      float w1 = dt_edge_fn(c, a, p);
      float w2 = dt_edge_fn(a, b, p);

      if (w0 >= 0.0f && w1 >= 0.0f && w2 >= 0.0f) {
        w0 /= area;
        w1 /= area;
        w2 /= area;

        // float r = w0 * c0[0] + w1 * c1[0] + w2 * c2[0];
        // float g = w0 * c0[1] + w1 * c1[1] + w2 * c2[1];
        // float b = w0 * c0[2] + w1 * c1[2] + w2 * c2[2];

        *pixel = packed;
      } else {
        *pixel = packed_inv;
      }

      ++pixel;
    }
    ptr += bmp->stride;
  }
}
