#include "aoc.h"

#define INPUT_LENGTH 2000

int main(void)
{
    String input = read_file("./inputs/day01.txt", INPUT_LENGTH);

    PrimitiveArrayList(uint32_t) array = new(PrimitiveArrayList(uint32_t), INPUT_LENGTH);

    for iter(String, it, input)
    {
        if (isdigit(it.value))
        {
            al_append(array, it.value - '0');
        }
    }

    del(String, input);

    uint32_t part1 = 0, part2 = 0;
    for iter(PrimitiveArrayList(uint32_t), it, array)
    {
        int32_t a = it.value;
        int32_t b = al_get(array, (it.index + 1) % array->length);
        int32_t c = al_get(array, (it.index + array->length / 2) % array->length);
        if (a == b)
        {
            part1 += a;
        }
        if (a == c)
        {
            part2 += a;
        }
    }

    del(PrimitiveArrayList(uint32_t), array);

    ANSWER(1136, part1, 1092, part2);
}
