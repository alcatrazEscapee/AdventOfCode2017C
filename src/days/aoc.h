// Common header file for all AoC puzzles

#ifndef AOC_H
#define AOC_H

#include <stdio.h> // printf, etc.
#include <stdlib.h> // malloc, free
#include <stdint.h> // int32_t, etc.
#include <stdbool.h> // true, false, bool
#include <stdarg.h> // Varargs

#include <string.h> // memcpy
#include <ctype.h> // isdigit
#include <limits.h> // INT_MAX, INT_MIN

// Required library elements
#include "../panic.h"

// Collections
#include "../collections/arrayhashmap.h"
#include "../collections/intarraylist.h"
#include "../math/integer.h"
#include "../math/vectors.h"

// Utils for consistient answers
#define ANSWER_INT(day, part, expected, actual) printf("Day %02d Part %d: Expected %d, Actual %d, Status = %s\n", day, part, expected, actual, ((expected) == (actual)) ? "Passed" : "Failed");
#define ANSWER_UINT(day, part, expected, actual) printf("Day %02d Part %d: Expected %u, Actual %u, Status = %s\n", day, part, expected, actual, ((expected) == (actual)) ? "Passed" : "Failed")

// Iterator macros
// These are intended to be used as for iter(name, ...) { ... }

#define file_lines__iter(file, line) for (char* line = NULL; read_line(file, &line);)
#define file_chars__iter(file, c) for (int32_t c = '\0'; (c = fgetc(file)) != EOF;)

// Iterates through strings, seperated by a specific token, in a parent string. The inner block has access to a char* token
// Use with iter(string_split, ...)
#define string_split__iter(string, split, token) for (char* token = strtok(string, split); token != NULL; token = strtok(NULL, split))

// Reads a single line from a file, not including the new line character
// If the passed in char** line is not NULL, it will be updated (possibly reallocated)
// If it is NULL, a new char* is allocated and ownership is given to the caller
// If the end of the file is reached (aka an empty string would be returned), this will return NULL
bool read_line(FILE* file, char** line);

#endif