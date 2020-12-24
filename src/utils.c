#include "utils.h"

uint32_t next_highest_power_of_two(uint32_t i)
{
    uint32_t n = i - 1;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    return n + 1;
}