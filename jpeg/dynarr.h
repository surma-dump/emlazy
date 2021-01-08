#ifndef DYNARR_H
#define DYNARR_H

#include <stdint.h>

typedef struct {
  unsigned long size;
  uint8_t *buffer;
} DynArr;

#endif // DYNARR_H