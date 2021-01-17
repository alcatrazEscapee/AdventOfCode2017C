#include "aoc.h"

#define Tuple Node, String, name, uint32_t, weight, ArrayList, children
#include "../lib/collections/tuple.template.c" 

int main(void)
{
    String input = read_file("./inputs/day07.txt", 1000);
    Map nodes = new(Map, 1024, class(String), class(Node));

    for iter(StringSplit, it, input, "\n") // Lines
    {
        uint32_t i0 = unwrap(str_index_of(it.value, " "));
        String key = str_substring(it.value, 0, i0);

        uint32_t i1 = unwrap(str_index_of(it.value, "("));
        uint32_t i2 = unwrap(str_index_of(it.value, ")"));
        String weight_string = str_substring(it.value, i1 + 1, i2);
        uint32_t weight = unwrap(str_parse_uint32_t(weight_string));
        del(String, weight_string);

        map_put(nodes, key, new(Node, copy(String, key), weight, new(ArrayList, 1, class(String))));
    }

    del(String, input);
    del(Map, nodes);
}
