// Vectors (the mathematical kind) of different dimensions

#ifndef VECTORS_H
#define VECTORS_H

#include <stdio.h> // printf, etc.
#include <stdlib.h> // malloc, free
#include <stdint.h> // int32_t, etc.
#include <stdbool.h> // true, false, bool

typedef struct {
    int32_t x;
    int32_t y;
} Vec2i;

#include "../class.h"
#include "../strings.h"

Vec2i* constructor(Vec2i)(int32_t x, int32_t y);
DERIVE_CLASS_HEADER(Vec2i, Vec2i*);

// Instance Methods

void v2i_set(Vec2i* p, int32_t x, int32_t y);

#endif