#include "utils.h"

static bool RNG_INITIALIZED = false;

#define ensure_initialized \
if (!RNG_INITIALIZED) \
{ \
    srand(time(NULL)); \
    RNG_INITIALIZED = true; \
}

#define RAND_16 ((uint32_t) (rand() & 0xFFFF))

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

uint32_t rand_uint32()
{
    ensure_initialized;
    return (((uint32_t) RAND_16) << 16) | ((uint32_t) RAND_16);
}

uint32_t rand_uint32_in(uint32_t upper_exclusive)
{
    return rand_uint32() % upper_exclusive;
}

void fill_array_nulls(void** array, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++)
    {
        array[i] = NULL;
    }
}