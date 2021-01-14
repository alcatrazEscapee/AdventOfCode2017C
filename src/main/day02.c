#include "aoc.h"

int main(void)
{
    String input = read_file("./inputs/day02.txt", 1000);
    PrimitiveArrayList(uint32_t) array = new(PrimitiveArrayList(uint32_t), 10);
    uint32_t part1 = 0, part2 = 0;

    for iter(StringSplit, line_it, input, "\n")
    {
        for iter(StringSplit, word_it, line_it.value, "\t")
        {
            al_append(array, str_parse_int32(word_it.value));
        }

        // Part 1 - calculate the difference between max and min in each line
        uint32_t min_value = INT_MAX, max_value = 0;
        for iter(PrimitiveArrayList(uint32_t), it, array)
        {
            min_value = min(min_value, it.value);
            max_value = max(max_value, it.value);
        }
        part1 += max_value - min_value;

        // Part 2 - find the only two numbers which divide one another, and find the quotient
        for iter(PrimitiveArrayList(uint32_t), ix, array)
        {
            for iter(PrimitiveArrayList(uint32_t), iy, array)
            {
                int32_t a = ix.value, b = iy.value;
                if (ix.index != iy.index && a > b && a % b == 0)
                {
                    part2 += a / b;
                }
            }
        }

        al_clear(array);
    }

    del(String, input);
    del(PrimitiveArrayList(uint32_t), array);

    ANSWER(54426, part1, 333, part2);
}