// Simple math util functions used by collections and other boilerplate code

#include "lib.h"

#ifndef UTILS_H
#define UTILS_H

uint32_t next_highest_power_of_two(uint32_t i);

uint32_t rand_uint32();
uint32_t rand_uint32_in(uint32_t upper_exclusive);

void fill_array_nulls(void** array, uint32_t size);

#endif