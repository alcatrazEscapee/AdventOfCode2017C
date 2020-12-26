#include "sorting.h"

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