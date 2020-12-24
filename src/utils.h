// Simple math util functions used by collections and other boilerplate code

#ifndef UTILS_H
#define UTILS_H

#include "std.h"

#define abs(x) ((x) < 0 ? -(x) : (x))
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

uint32_t next_highest_power_of_two(uint32_t i);

#endif