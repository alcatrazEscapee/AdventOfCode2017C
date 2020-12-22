// Common header file for all AoC puzzles

#ifndef AOC_H
#define AOC_H

#include <stdio.h> // printf, etc.
#include <stdlib.h> // malloc, free
#include <stdint.h> // int32_t, etc.
#include <stdbool.h> // true, false, bool
#include <string.h> // memcpy
#include <ctype.h> // isdigit

// If this happens, there's a bug in your code
#define PANIC(args...) { printf("PANIC!\n"); printf(args); exit(1); }
#define PANIC_IF_NULL(value, args...) { if (value == NULL) { PANIC(args); }}

#define null NULL

// Utils for consistient answers
#define ANSWER_INT(day, part, expected, actual) printf("Day %02d Part %d: Expected %d, Actual %d, Status = %s\n", day, part, expected, actual, ((expected) == (actual)) ? "Passed" : "Failed");

// We like new language features
#define new(class, args...) class ## __new(args)
#define del(class, instance) class ## __del(instance)

#endif