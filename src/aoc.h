// Common header file for all AoC puzzles

#ifndef AOC_H
#define AOC_H

#include <stdio.h> // printf, etc.
#include <stdlib.h> // malloc, free
#include <stdint.h> // int32_t, etc.
#include <stdbool.h> // true, false, bool
#include <string.h> // memcpy
#include <ctype.h> // isdigit
#include <limits.h> // INT_MAX, INT_MIN

// If this happens, there's a bug in your code
#define PANIC(args...) { printf("PANIC!\n"); printf(args); exit(1); }
#define PANIC_IF_NULL(value, args...) { if (value == NULL) { PANIC(args); }}

// Utils for consistient answers
#define ANSWER_INT(day, part, expected, actual) printf("Day %02d Part %d: Expected %d, Actual %d, Status = %s\n", day, part, expected, actual, ((expected) == (actual)) ? "Passed" : "Failed");

// We like new language features
#define new(class, args...) class ## __new(args)
#define del(class, instance) class ## __del(instance)
#define iter(class, args...) class ## __iter(args)

// Iterator macros
// These are intended to be used as for iter(name, ...) { ... }

// Iterates through lines (separated by newline) in a file. The inner block has access to a char* line
// Use with iter(file_lines, ...)
#define file_lines__iter(file, line) (char* line = NULL; read_line(file, &line);)

// Iterates through strings, seperated by a specific token, in a parent string. The inner block has access to a char* token
// Use with iter(string_split, ...)
#define string_split__iter(string, split, token) (char* token = strtok(string, split); token != NULL; token = strtok(NULL, split))

// Reads a single line from a file, not including the new line character
// If the passed in char** line is not NULL, it will be updated (possibly reallocated)
// If it is NULL, a new char* is allocated and ownership is given to the caller
// If the end of the file is reached (aka an empty string would be returned), this will return NULL
bool read_line(FILE* file, char** line);

#endif