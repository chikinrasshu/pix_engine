#pragma once

#include "common.h"

typedef struct file {
  u8 *memory;
  u64 size;
} file;

b32 load_file(file *f, cstr path);
b32 destroy_file(file *f);
