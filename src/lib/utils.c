#include "utils.h"


// String Printing

void println(slice_t format_string, ...)
{
    va_list arg;
    va_start(arg, format_string);
    vprintf(format_string, arg);
    va_end(arg);
    printf("\n");
}

void str_print(String string)
{
    printf("%s", string->slice);
    del(String, string);
}

void str_println(String string)
{
    printf("%s\n", string->slice);
    del(String, string);
}


// Random Number Generators

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

// Sorting

void sorting_qsort_recursive(void* instance, FnSortingLessThan lt_fn, FnSortingSwap swap_fn, int32_t low, int32_t high)
{
    if (low < high)  
    {  
        uint32_t pi = sorting_qsort_partition(instance, lt_fn, swap_fn, low, high);  
  
        sorting_qsort_recursive(instance, lt_fn, swap_fn, low, pi - 1);  
        sorting_qsort_recursive(instance, lt_fn, swap_fn, pi + 1, high);  
    }  
}

uint32_t sorting_qsort_partition(void* instance, FnSortingLessThan lt_fn, FnSortingSwap swap_fn, int32_t low, int32_t high)
{
    int32_t i = (low - 1); // Index of smaller element
    for (int32_t j = low; j <= high - 1; j++)
    {
        // If current element is smaller than the pivot
        if (lt_fn(instance, j, high))
        {
            i++; // increment index of smaller element
            swap_fn(instance, i, j);
        }
    }
    swap_fn(instance, i + 1, high);
    return (i + 1);
}