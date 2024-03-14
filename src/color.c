#include "color.h"

u32 pack_rgba(u8 r, u8 g, u8 b, u8 a) {
  return (r << 16) | (g << 8) | (b << 0) | (a << 24);
}

u32 pack_rgb(u8 r, u8 g, u8 b) { return pack_rgba(r, g, b, 255); }
