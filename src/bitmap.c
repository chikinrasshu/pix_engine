#include "bitmap.h"
#include "color.h"

#include <stdio.h>
#include <stdlib.h>

b32 debug_bitmap(bitmap *bmp) {
  if (!bmp) {
    printf("bmp was NULL\n");
    return false;
  }

  u8 *ptr = bmp->memory;
  for (s32 y = 0; y < bmp->size.y; ++y) {
    u32 *pixel = (u32 *)ptr;
    for (s32 x = 0; x < bmp->size.x; ++x) {
      *pixel++ = pack_rgb(x, y, 0);
    }
    ptr += bmp->stride;
  }

  return true;
}

b32 create_bitmap(bitmap *bmp, v2i size, s32 bpp) {
  if (!bmp) {
    printf("bmp was NULL\n");
    return false;
  }
  if (size.x <= 0 || size.y <= 0) {
    printf("Invalid bitmap dimensions %dx%d\n", size.x, size.y);
    return false;
  }

  if (bpp <= 0) {
    printf("bmp->bpp was <= 0\n");
    return false;
  }

  bmp->size = size;
  bmp->bpp = bpp;
  bmp->stride = bmp->size.x * sizeof(u8) * bmp->bpp;
  bmp->memory_size = bmp->stride * bmp->size.y;
  bmp->memory = malloc(bmp->memory_size);

  printf("Created bitmap %dx%d@%dbpp\n", bmp->size.x, bmp->size.y, bmp->bpp);
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

b32 resize_bitmap(bitmap *bmp, v2i size) {
  if (!bmp) {
    printf("bmp was NULL\n");
    return false;
  }

  if (bmp->bpp <= 0) {
    printf("bmp->bpp was <= 0\n");
    return false;
  }

  bmp->size = size;
  bmp->stride = bmp->size.x * sizeof(u8) * bmp->bpp;
  bmp->memory_size = bmp->stride * bmp->size.y;
  bmp->memory = realloc(bmp->memory, bmp->memory_size);

  printf("Resized bitmap to %dx%d@%dbpp\n", bmp->size.x, bmp->size.y, bmp->bpp);
  debug_bitmap(bmp);

  return bmp->memory != NULL;
}

b32 clear_bitmap(bitmap *bmp, u32 clear_color) {
  if (!bmp) {
    printf("bmp was NULL\n");
    return false;
  }

  u8 *ptr = bmp->memory;
  for (s32 y = 0; y < bmp->size.y; ++y) {
    u32 *pixel = (u32 *)ptr;
    for (s32 x = 0; x < bmp->size.x; ++x) {
      *pixel++ = clear_color;
    }
    ptr += bmp->stride;
  }

  return true;
}
