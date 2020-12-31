// Vectors (the mathematical kind) of different dimensions

#include "../common.h"

#ifndef VECTORS_H
#define VECTORS_H

typedef struct {
    int32_t x;
    int32_t y;
} Vec2i;

Vec2i* Vec2i__new(int32_t x, int32_t y);
DERIVE_CLASS_HEADER(Vec2i, Vec2i*);

// Instance Methods

void v2i_set(Vec2i* p, int32_t x, int32_t y);

#endif