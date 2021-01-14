// Simple math util functions used by collections and other boilerplate code

#include "lib.h"

#ifndef UTILS_H
#define UTILS_H

// File IO

String read_file(slice_t file_name, uint32_t estimated_size);

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

typedef bool (*FnSortingLessThan) (pointer_t, uint32_t, uint32_t);
typedef void (*FnSortingSwap) (pointer_t, uint32_t, uint32_t);

void sorting_qsort_recursive(pointer_t instance, FnSortingLessThan lt_fn, FnSortingSwap swap_fn, uint32_t low, uint32_t high);
uint32_t sorting_qsort_partition(pointer_t instance, FnSortingLessThan lt_fn, FnSortingSwap swap_fn, uint32_t low, uint32_t high);

#endif