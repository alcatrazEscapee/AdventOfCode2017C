// Simple math util functions used by collections and other boilerplate code

#include "lib.h"

#ifndef UTILS_H
#define UTILS_H

// String Printing
// Adds println, and variants which accept Strings destructively.

void println(slice_t format_string, ...); // printf, but with a '\n' appended to the output

void str_print(String string); // Destructive printing of a string. For non-destructive behavior, write printf("%s", string->slice);
void str_println(String string); // Destructive printing of a string. For non-destructive behavor, write println("%s\n", string->slice);

// Math

uint32_t next_highest_power_of_two(uint32_t i);

uint32_t rand_uint32();
uint32_t rand_uint32_in(uint32_t upper_exclusive);

// Sorting

typedef bool (*FnSortingLessThan) (void*, uint32_t, uint32_t);
typedef void (*FnSortingSwap) (void*, uint32_t, uint32_t);

void sorting_qsort_recursive(void* instance, FnSortingLessThan lt_fn, FnSortingSwap swap_fn, int32_t low, int32_t high);
uint32_t sorting_qsort_partition(void* instance, FnSortingLessThan lt_fn, FnSortingSwap swap_fn, int32_t low, int32_t high);

#endif