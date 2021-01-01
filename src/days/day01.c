#include "aoc.h"

#define INPUT_LENGTH 2000

int main(void)
{
    String* input = read_file("./inputs/day01.txt", INPUT_LENGTH);
    IntArrayList* array = new(IntArrayList, INPUT_LENGTH);
    
    str_remove_whitespace(input);
    for iter(String, it, input)
    {
        ial_append(array, it.value - '0');
    }

    del(String, input);

    int32_t part1 = 0, part2 = 0;
    for iter(IntArrayList, it, array)
    {
        int32_t a = it.value;
        int32_t b = ial_get(array, (it.index + 1) % array->length);
        int32_t c = ial_get(array, (it.index + array->length / 2) % array->length);
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
