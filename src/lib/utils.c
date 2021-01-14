#include "utils.h"

// File IO

String read_file(slice_t file_name, uint32_t estimated_size)
{
    FILE* file = fopen(file_name, "r");
    panic_if_null(file, "Unable to open file '%s'", file_name);

    String s = str_create_with_length(estimated_size);
    char c = '\0';
    while ((c = fgetc(file)) != EOF)
    {
        if (c != '\r')
        {
            str_append_char(s, c);
        }
    }

    fclose(file);

    return s;
}


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

void sorting_qsort_recursive(pointer_t instance, FnSortingLessThan lt_fn, FnSortingSwap swap_fn, uint32_t low, uint32_t high)
{
    // Minor optimization: sort cases of one or two elements directly
    switch (high - low)
    {
        case 1:
            if (lt_fn(instance, high, low))
            {
                swap_fn(instance, high, low);
            }
        case 0:
            return;
    }
    // All other cases, recursive qsort.
    // Guard conditions against underflow when low = pi or pi = high
    uint32_t pi = sorting_qsort_partition(instance, lt_fn, swap_fn, low, high);  
    if (pi - low > 1)
    {
        sorting_qsort_recursive(instance, lt_fn, swap_fn, low, pi - 1);  
    }
    if (high - pi > 1)
    {
        sorting_qsort_recursive(instance, lt_fn, swap_fn, pi + 1, high);
    }
}

uint32_t sorting_qsort_partition(pointer_t instance, FnSortingLessThan lt_fn, FnSortingSwap swap_fn, uint32_t low, uint32_t high)
{
    // Take the last element to be the pivot
    // Avoid issues with underflow by taking i = low rather than low - 1 as seen in some examples
    uint32_t i = low;
    for (uint32_t j = low; j < high; j++)
    {
        // If current element is smaller than the pivot
        if (lt_fn(instance, j, high))
        {
            // swap and increment index of smaller element
            swap_fn(instance, i, j);
            i++;
        }
    }
    swap_fn(instance, i, high);
    return i;
}