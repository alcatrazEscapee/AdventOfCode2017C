#include "aoc.h"

int main(void)
{
    String* input = read_file("./inputs/day02.txt", 1000);
    ArrayList* lines = str_split_lines(input);    
    IntArrayList* array = new(IntArrayList, 10);

    del(String, input);

    int part1 = 0, part2 = 0;
    iter(ArrayList, String*, lines, i, line)
    {
        ArrayList* values = str_split_whitespace(line);
        iter(ArrayList, String*, values, j, substr)
        {
            ial_append(array, str_parse_int32(substr));
        }
        del(ArrayList, values);

        // Part 1 - calculate the difference between max and min in each line
        int32_t min_value = INT_MAX, max_value = INT_MIN;
        iter(IntArrayList, array, i, next)
        {
            min_value = min(min_value, next);
            max_value = max(max_value, next);
        }
        part1 += max_value - min_value;

        // Part 2 - find the only two numbers which divide one another, and find the quotient
        iter(IntArrayList, array, i, a)
        {
            iter(IntArrayList, array, j, b)
            {
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