
#include "unittest.h"


// Sorting functions for int arrays
void swap(PrimitiveArrayList(uint32_t) array, uint32_t i, uint32_t j)
{
    uint32_t t = al_get(array, i);
    al_set(array, i, al_get(array, j));
    al_set(array, j, t);
}

bool lt(PrimitiveArrayList(uint32_t) array, uint32_t i, uint32_t j)
{
    return al_get(array, i) < al_get(array, j);
}

TEST(test_utils_qsort_array_list, {
    // Stress tests for qsort algorithim
    PrimitiveArrayList(uint32_t) array = new(PrimitiveArrayList(uint32_t), 10);

    // Different size lists
    for (uint32_t k = 0; k < 5; k++)
    {
        uint32_t size = 3 + (1 << (2 * k));

        // Iterations
        for (uint32_t i = 0; i < 1000; i++)
        {
            // Initialize array randomly
            al_clear(array);
            for (uint32_t j = 0; j < size; j++)
            {
                al_append(array, rand_uint32());
            }

            // Sort
            sorting_qsort_recursive(array, (FnSortingLessThan) & lt, (FnSortingSwap) & swap, 0, size - 1);

            // Verify sorted
            for (uint32_t j = 0; j < size - 1; j++)
            {
                ASSERT_TRUE(al_get(array, j) <= al_get(array, j + 1), "Array is not sorted: '%s'", format(PrimitiveArrayList(uint32_t), array)->slice);
            }
        }
    }

    del(PrimitiveArrayList(uint32_t), array);
});

TEST_GROUP(test_utils, {
    test_utils_qsort_array_list();
});