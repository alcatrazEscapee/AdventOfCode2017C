#include "aoc.h"

int main(void)
{
    String* input = read_file("./inputs/day04.txt", 1000);
    ArrayList* lines = str_split_lines(input);

    ArrayHashSet* unique_words = new(ArrayHashSet, 10, class(String));
    ArrayHashSet* unique_sorted_words = new(ArrayHashSet, 10, class(String));

    del(String, input);

    uint32_t part1 = 0, part2 = 0;
    iter(ArrayList, String*, lines, i, line)
    {
        ArrayList* words = str_split_whitespace(line);

        bool part1_valid = true, part2_valid = true;
        iter(ArrayList, String*, words, j, word)
        {
            // The array list currently owns 'word', so we need to copy it to transfer ownership to the ArrayHashSet
            if (ahs_put(unique_words, copy(String, word)))
            {
                part1_valid = false;
                part2_valid = false;
                break;
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

        del(ArrayList, words);

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

    del(ArrayList, lines);
    del(ArrayHashSet, unique_words);
    del(ArrayHashSet, unique_sorted_words);

    ANSWER_UINT(4, 1, 325, part1);
    ANSWER_UINT(4, 2, 119, part2);

    return 0;
}