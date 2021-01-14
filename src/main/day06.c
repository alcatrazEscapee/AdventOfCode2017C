#include "aoc.h"

int32_t INPUT[] = {10, 3, 15, 10, 5, 15, 5, 15, 9, 2, 5, 8, 5, 2, 3, 6};
#define SIZE 16

int main(void)
{
    uint32_t part1 = 0;
    uint32_t part2 = 0;

    uint32_t mask = SIZE - 1; // This only works when SIZE is a multiple of 2
    panic_if(next_highest_power_of_two(SIZE) != SIZE, "This solution uses bit fiddling hacks that only work with SIZE = 1 << N");

    Map found = new(Map, 1000, class(PrimitiveArrayList(uint32_t)), class(Int32));
    PrimitiveArrayList(uint32_t) state = new(PrimitiveArrayList(uint32_t), SIZE);
    for (uint32_t i = 0; i < SIZE; i++)
    {
        al_append(state, INPUT[i]);
    }

    uint32_t cycle = 0;
    map_put(found, state, new(Int32, cycle));
    loop
    {
        PrimitiveArrayList(uint32_t) next = copy(PrimitiveArrayList(uint32_t), state);

        // Compute the smallest memory bank
        uint32_t max_index = 0;
        uint32_t max_value = al_get(next, 0);
        for iter(PrimitiveArrayList(uint32_t), it, next)
        {
            if (it.value > max_value)
            {
                max_value = it.value;
                max_index = it.index;
            }
        }

        // Remove all from the minimum bank
        al_set(next, max_index, 0);

        // Distribute that memory into all other banks
        // This could be optimized if the banks had absurdly large values, but since we're only dealing with ~ the same amount per bank, we don't expect to iterate this loop much longer than nessecary
        uint32_t index = (max_index + 1) & mask;
        for (uint32_t i = 0; i < max_value; i++)
        {
            next->values[index]++;
            index = (index + 1) & mask;
        }

        cycle++;

        if (map_contains_key(found, next))
        {
            // Detected a cycle
            part1 = cycle;
            part2 = cycle - *(Int32) map_get(found, next);

            del(PrimitiveArrayList(uint32_t), next);
            break;
        }

        map_put(found, next, new(Int32, cycle));
        state = next;
    }

    del(Map, found);

    ANSWER(14029, part1, 2765, part2);
}