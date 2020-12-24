#include "aoc.h"

int main(void)
{
    FILE* file = fopen("./inputs/day01.txt", "r");
    PANIC_IF_NULL(file, "Unable to open input file.");

    IntArrayList* array = new(IntArrayList, 2098);
    iter(file_chars, file, c)
    {
        if (isdigit(c))
        {
            ial_append(array, c - '0');
        }
    }

    fclose(file);

    int32_t part1 = 0, part2 = 0;
    iter(IntArrayList, array, i, a)
    {
        int32_t b = ial_get(array, (i + 1) % array->length);
        int32_t c = ial_get(array, (i + array->length / 2) % array->length);
        if (a == b)
        {
            part1 += a;
        }
        if (a == c)
        {
            part2 += a;
        }
    }

    del(IntArrayList, array);

    ANSWER_INT(1, 1, 1136, part1);
    ANSWER_INT(1, 2, 1092, part2);

    return 0;
}
