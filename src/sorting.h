// Collections of generic (using function pointers and class objects) sorting algorithims

#include "lib.h"

#ifndef SORTING_H
#define SORTING_H

typedef bool (*FnSortingLessThan) (void*, uint32_t, uint32_t);
typedef void (*FnSortingSwap) (void*, uint32_t, uint32_t);

void sorting_qsort_recursive(void* instance, FnSortingLessThan lt_fn, FnSortingSwap swap_fn, int32_t low, int32_t high);
uint32_t sorting_qsort_partition(void* instance, FnSortingLessThan lt_fn, FnSortingSwap swap_fn, int32_t low, int32_t high);

#endif