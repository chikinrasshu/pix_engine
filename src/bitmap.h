#pragma once

#include "common.h"

typedef struct bitmap {
  s32 w, h, bpp;
  u8 *memory;
  u64 memory_size;
  u64 stride;
} bitmap;

b32 create_bitmap(bitmap *bmp, s32 w, s32 h, s32 bpp);
b32 destroy_bitmap(bitmap *bmp);

b32 resize_bitmap(bitmap *bmp, s32 w, s32 h);
