#include "aoc.h"

int32_t INPUT[] = {10, 3, 15, 10, 5, 15, 5, 15, 9, 2, 5, 8, 5, 2, 3, 6};
#define SIZE 16

int main(void)
{
    uint32_t part1 = 0;
    uint32_t part2 = 0;

    uint32_t mask = SIZE - 1; // This only works when SIZE is a multiple of 2
    PANIC_IF(next_highest_power_of_two(SIZE) != SIZE, "This solution uses bit fiddling hacks that only work with SIZE = 1 << N");

    ArrayHashMap* found = new(ArrayHashMap, 1000, class(IntArrayList), class(Int32));
    IntArrayList* state = ial_create_from_array(INPUT, SIZE);

    uint32_t cycle = 0;
    ahm_put(found, state, new(Int32, cycle));
    loop
    {
        IntArrayList* next = copy(IntArrayList, state);

        // Compute the smallest memory bank
        uint32_t max_index = 0;
        int32_t max_value = ial_get(next, 0);
        for iter(IntArrayList, it, next)
        {
            if (it.value > max_value)
            {
                max_value = it.value;
                max_index = it.index;
            }
        }

        // Remove all from the minimum bank
        ial_set(next, max_index, 0);

        // Distribute that memory into all other banks
        // This could be optimized if the banks had absurdly large values, but since we're only dealing with ~ the same amount per bank, we don't expect to iterate this loop much longer than nessecary
        uint32_t index = (max_index + 1) & mask;
        for (int32_t i = 0; i < max_value; i++)
        {
            next->values[index]++;
            index = (index + 1) & mask;
        }

        cycle++;

        if (ahm_key_in(found, next))
        {
            // Detected a cycle
            part1 = cycle;
            part2 = cycle - *(int32_t*) ahm_get(found, next);

            del(IntArrayList, next);
            break;
        }

        ahm_put(found, next, new(Int32, cycle));
        state = next;
    }

    ANSWER_UINT(6, 1, 14029, part1);
    ANSWER_UINT(6, 2, 2765, part2);

    del(ArrayHashMap, found);

    return 0;
}