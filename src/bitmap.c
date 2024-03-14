#include "bitmap.h"

#include <stdio.h>
#include <stdlib.h>

u32 pack_rgba(u8 r, u8 g, u8 b, u8 a) {
  return (r << 16) | (g << 8) | (b << 0) | (a << 24);
}

u32 pack_rgb(u8 r, u8 g, u8 b) { return pack_rgba(r, g, b, 255); }

b32 debug_bitmap(bitmap *bmp) {
  if (!bmp) {
    printf("bmp was NULL\n");
    return false;
  }

  u8 *ptr = bmp->memory;
  for (s32 y = 0; y < bmp->h; ++y) {
    u32 *pixel = (u32 *)ptr;
    for (s32 x = 0; x < bmp->w; ++x) {
      *pixel++ = pack_rgb(x, y, 0);
    }
    ptr += bmp->stride;
  }

  return true;
}

b32 create_bitmap(bitmap *bmp, s32 w, s32 h, s32 bpp) {
  if (!bmp) {
    printf("bmp was NULL\n");
    return false;
  }
  if (w <= 0 || h <= 0) {
    printf("Invalid bitmap dimensions %dx%d\n", w, h);
    return false;
  }

  if (bpp <= 0) {
    printf("bmp->bpp was <= 0\n");
    return false;
  }

  bmp->w = w;
  bmp->h = h;
  bmp->bpp = bpp;
  bmp->stride = bmp->w * sizeof(u8) * bmp->bpp;
  bmp->memory_size = bmp->stride * bmp->h;
  bmp->memory = malloc(bmp->memory_size);

  printf("Created bitmap %dx%d@%dbpp\n", bmp->w, bmp->h, bmp->bpp);
  debug_bitmap(bmp);

  return bmp->memory != NULL;
}

b32 destroy_bitmap(bitmap *bmp) {
  if (!bmp) {
    printf("bmp was NULL\n");
    return false;
  }

  free(bmp->memory);
  bmp->memory = NULL;
  bmp->memory_size = 0;

  return true;
}

b32 resize_bitmap(bitmap *bmp, s32 w, s32 h) {
  if (!bmp) {
    printf("bmp was NULL\n");
    return false;
  }

  if (bmp->bpp <= 0) {
    printf("bmp->bpp was <= 0\n");
    return false;
  }

  bmp->w = w;
  bmp->h = h;
  bmp->stride = w * sizeof(u8) * bmp->bpp;
  bmp->memory_size = bmp->stride * bmp->h;
  bmp->memory = realloc(bmp->memory, bmp->memory_size);

  printf("Resized bitmap to %dx%d@%dbpp\n", bmp->w, bmp->h, bmp->bpp);
  debug_bitmap(bmp);

  return bmp->memory != NULL;
}
