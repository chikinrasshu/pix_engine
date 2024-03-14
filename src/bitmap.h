#pragma once

#include "common.h"
#include "vec.h"

typedef struct bitmap {
  v2i size;
  s32 bpp;
  u8 *memory;
  u64 memory_size;
  u64 stride;
} bitmap;

b32 create_bitmap(bitmap *bmp, v2i size, s32 bpp);
b32 destroy_bitmap(bitmap *bmp);

b32 resize_bitmap(bitmap *bmp, v2i size);
b32 clear_bitmap(bitmap *bmp, u32 clear_color);
