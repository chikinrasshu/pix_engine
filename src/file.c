#include "file.h"

#include <stdio.h>
#include <stdlib.h>

b32 load_file(file *f, cstr path) {
  if (!f) {
    printf("f was NULL\n");
    return false;
  }

  if (!path) {
    printf("path was NULL\n");
    return false;
  }

  FILE *fptr = fopen(path, "rb");
  if (!fptr) {
    printf("Failed to open '%s'\n", path);
    return false;
  }

  fseek(fptr, 0, SEEK_END);
  u32 fsize = ftell(fptr);
  fseek(fptr, 0, SEEK_SET);

  f->memory = malloc(fsize);
  if (!f->memory) {
    printf("Failed to allocate %u bytes for file '%s'\n", fsize, path);
    return false;
  }

  u32 bytes_read = fread(f->memory, fsize, 1, fptr);
  if (bytes_read != 1) {
    printf("Failed to read %u bytes from file '%s'\n", fsize, path);
    return false;
  }

  fclose(fptr);

  f->memory[fsize] = '\0';
  f->size = (u64)fsize;
  return f->memory != NULL;
}

b32 destroy_file(file *f) {
  if (!f) {
    printf("f was NULL\n");
    return false;
  }

  if (f->memory) {
    free(f->memory);
    f->memory = NULL;
  }
  f->size = 0;

  return true;
}
