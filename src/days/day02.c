#include "../aoc.h"
#include "../utils/intarraylist.h"

int main(void)
{
    FILE* file = fopen("./inputs/day02.txt", "r");
    PANIC_IF_NULL(file, "Unable to open input file.");

    IntArrayList* array = new(IntArrayList, 10);

    int part1 = 0, part2 = 0;
    for iter(file_lines, file, line)
    {
        for iter(string_split, line, "\t", value)
        {
            int e = 0;
            sscanf(value, "%d", &e);
            ial_append(array, e);
        }

        // Part 1 - calculate the difference between max and min in each line
        int32_t min = INT_MAX, max = INT_MIN;
        int32_t next;
        for iter(IntArrayList, array, i, next)
        {
            min = min < next ? min : next;
            max = max > next ? max : next;
        }
        part1 += max - min;

        // Part 2 - find the only two numbers which divide one another, and find the quotient
        int32_t a, b;
        for iter(IntArrayList, array, i, a)
        {
            for iter(IntArrayList, array, j, b)
            {
                if (a != b && a > b && a % b == 0)
                {
                    part2 += a / b;
                }
            }
        }

        ial_clear(array);
    }

    del(IntArrayList, array);
    fclose(file);

    ANSWER_INT(2, 1, 54426, part1);
    ANSWER_INT(2, 2, 333, part2);

    return 0;
}