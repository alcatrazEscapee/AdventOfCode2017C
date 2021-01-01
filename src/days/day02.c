#include "aoc.h"

int main(void)
{
    String* input = read_file("./inputs/day02.txt", 1000);
    ArrayList* lines = str_split_lines(input);    
    IntArrayList* array = new(IntArrayList, 10);

    del(String, input);

    int part1 = 0, part2 = 0;
    for iter(ArrayList, line_it, lines)
    {
        ArrayList* values = str_split_whitespace(line_it.value);
        for iter(ArrayList, it, values)
        {
            ial_append(array, str_parse_int32(it.value));
        }
        del(ArrayList, values);

        // Part 1 - calculate the difference between max and min in each line
        int32_t min_value = INT_MAX, max_value = INT_MIN;
        for iter(IntArrayList, it, array)
        {
            min_value = min(min_value, it.value);
            max_value = max(max_value, it.value);
        }
        part1 += max_value - min_value;

        // Part 2 - find the only two numbers which divide one another, and find the quotient
        for iter(IntArrayList, ix, array)
        {
            for iter(IntArrayList, iy, array)
            {
                int32_t a = ix.value, b = iy.value;
                if (a != b && a > b && a % b == 0)
                {
                    part2 += a / b;
                }
            }
        }

        ial_clear(array);
    }

    del(ArrayList, lines);
    del(IntArrayList, array);

    ANSWER_INT(2, 1, 54426, part1);
    ANSWER_INT(2, 2, 333, part2);

    return 0;
}