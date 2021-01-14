// Common header file for all AoC puzzles

#include "../lib/lib.h"

// Answer utils
// Uses a clever hack with __FILE__ to get the day, assuming it matches dayXX.c
#define ANSWER(part1_expected, part1_actual, part2_expected, part2_actual) do { \
    String __day = new(String, __FILE__); \
    char __buf[3] = {__day->slice[__day->length - 4], __day->slice[__day->length - 3], '\0'}; \
    bool __error = false; \
    ANSWER_PART(__error,__buf, 1, part1_expected, part1_actual); \
    ANSWER_PART(__error, __buf, 2, part2_expected, part2_actual); \
    del(String, __day); \
    return __error; \
} while(0)


#define ANSWER_PART(error, day, part, expected, actual) do { \
    String __part = str_format("Day %s Part 1: Expected ", day); \
    bool __passed = ANSWER_EQUALS(expected, actual); \
    error |= !__passed; \
    str_append(__part, expected); \
    str_append(__part, ", Actual "); \
    str_append(__part, actual); \
    str_append(__part, __passed ? " : Passed" : " : Failed"); \
    str_println(__part); \
} while (0)

#define ANSWER_EQUALS(expected, actual) ((bool) _Generic((actual), \
    String : equals(String, ((String)(intptr_t)(expected)), ((String)(intptr_t)(actual))), \
    default: ((expected) == (actual)) \
))
