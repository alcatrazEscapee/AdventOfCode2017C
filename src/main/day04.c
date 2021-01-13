#include "aoc.h"

int main(void)
{
    String* input = read_file("./inputs/day04.txt", 1000);

    ArrayHashSet* unique_words = new(ArrayHashSet, 10, class(String));
    ArrayHashSet* unique_sorted_words = new(ArrayHashSet, 10, class(String));

    uint32_t part1 = 0, part2 = 0;
    for iter(StringSplit, line_it, input, "\n")
    {
        bool part1_valid = true, part2_valid = true;

        for iter(StringSplit, word_it, line_it.value, " ")
        {
            String* word = move(word_it.value);
            if (ahs_put(unique_words, word))
            {
                part1_valid = false;
                part2_valid = false;
            }

            if (part2_valid)
            {
                String* sorted_word = copy(String, word);
                str_sort(sorted_word);
                if (ahs_put(unique_sorted_words, sorted_word))
                {
                    part2_valid = false;
                }
            }
        }

        if (part1_valid)
        {
            part1++;
        }
        if (part2_valid)
        {
            part2++;
        }

        ahs_clear(unique_words);
        ahs_clear(unique_sorted_words);
    }

    del(String, input);
    del(ArrayHashSet, unique_words);
    del(ArrayHashSet, unique_sorted_words);

    ANSWER_UINT(4, 1, 325, part1);
    ANSWER_UINT(4, 2, 119, part2);

    return 0;
}