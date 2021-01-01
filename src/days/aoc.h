// Common header file for all AoC puzzles

#ifndef AOC_H
#define AOC_H

#include "../common.h"

// Utils for consistient answers
#define ANSWER_INT(day, part, expected, actual) printf("Day %02d Part %d: Expected %d, Actual %d, Status = %s\n", day, part, expected, actual, ((expected) == (actual)) ? "Passed" : "Failed")
#define ANSWER_UINT(day, part, expected, actual) printf("Day %02d Part %d: Expected %u, Actual %u, Status = %s\n", day, part, expected, actual, ((expected) == (actual)) ? "Passed" : "Failed")

// I/O Utils, typically used to read the input

String* read_file(char* file_name, uint32_t estimated_size);

#endif