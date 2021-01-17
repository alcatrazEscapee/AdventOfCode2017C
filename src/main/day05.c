#include "aoc.h"

int main(void)
{
    String input = read_file("./inputs/day05.txt", 1000);
    PrimitiveArrayList(int32_t) code = new(PrimitiveArrayList(int32_t), 1000);
    
    for iter(StringSplit, it, input, "\n")
    {
        al_append(code, unwrap(str_parse(int32_t, it.value)));
    }

    del(String, input);
    PrimitiveArrayList(int32_t) active_code = copy(PrimitiveArrayList(int32_t), code);

    int32_t pointer = 0;
    uint32_t part1 = 0;
    while (pointer >= 0 && pointer < (int32_t) active_code->length)
    {
        int32_t value = al_get(active_code, pointer);
        al_set(active_code, pointer, value + 1);
        pointer += value;
        part1++;
    }

    del(PrimitiveArrayList(int32_t), active_code);
    active_code = copy(PrimitiveArrayList(int32_t), code);
    
    uint32_t part2 = 0;
    pointer = 0;

    while (pointer >= 0 && pointer < (int32_t) active_code->length)
    {
        int32_t value = al_get(active_code, pointer);
        int32_t offset = value >= 3 ? -1 : 1;
        al_set(active_code, pointer, value + offset);
        pointer += value;
        part2++;
    }

    del(PrimitiveArrayList(int32_t), code);
    del(PrimitiveArrayList(int32_t), active_code);

    ANSWER(343467, part1, 24774780, part2);
}