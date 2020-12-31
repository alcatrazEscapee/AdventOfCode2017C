#include "aoc.h"

int main(void)
{
    String* input = read_file("./inputs/day05.txt", 1000);
    ArrayList* lines = str_split_whitespace(input);

    del(String, input);

    IntArrayList* code = new(IntArrayList, 1000);
    iter(ArrayList, String*, lines, i, line)
    {
        ial_append(code, str_parse_int32(line));
    }

    del(ArrayList, lines);

    IntArrayList* active_code = copy(IntArrayList, code);

    int32_t pointer = 0;
    uint32_t part1 = 0;
    while (pointer >= 0 && pointer < (int32_t) active_code->length)
    {
        int32_t value = ial_get(active_code, pointer);
        ial_set(active_code, pointer, value + 1);
        pointer += value;
        part1++;
    }

    del(IntArrayList, active_code);
    active_code = copy(IntArrayList, code);
    
    uint32_t part2 = 0;
    pointer = 0;

    while (pointer >= 0 && pointer < (int32_t) active_code->length)
    {
        int32_t value = ial_get(active_code, pointer);
        int32_t offset = value >= 3 ? -1 : 1;
        ial_set(active_code, pointer, value + offset);
        pointer += value;
        part2++;
    }

    del(IntArrayList, code);
    del(IntArrayList, active_code);

    ANSWER_UINT(5, 1, 343467, part1);
    ANSWER_UINT(5, 2, 24774780, part2);

    return 0;
}