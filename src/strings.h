// A heap allocated String data type with utility methods, similar to Rust's String vs. &'static str
// Unlike Rust's string natively handling unicode data, we make the (simplifying) assumptions that a character represents a single byte of 7-bit ASCII data.

#ifndef STRINGS_H
#define STRINGS_H

#include <stdio.h> // printf, etc.
#include <stdlib.h> // malloc, free
#include <stdint.h> // int32_t, etc.
#include <stdbool.h> // true, false, bool
#include <string.h> // memcpy
#include <stdarg.h> // Varargs

typedef struct String__struct
{
    char* slice; // Backing array
    uint32_t size; // Size of backing array
    /* public readonly */ uint32_t length; // Length of filled string (number of single byte ASCII chars)
} String;

#include "class.h"
#include "panic.h"
#include "sorting.h"
#include "utils.h"
#include "collections/arraylist.h"

// Class Definitions
String* constructor(String)(char* initial_value);
DERIVE_CLASS_HEADER(String, String*);

// Iterator
#define String__iter(string, index, character) uint32_t index = 0; for (char character = '\0'; index < string->length ? (character = string->slice[index], true) : false; index++)

// String printing. Adds println for both varargs and String variants, and print for Strings
// All variants using a String consume the string
// Both variants that take a String first store the provided expression (e.g. if it was the result of a format() call) as to not duplicate it.

#define println(args...) do { \
    printf(args); \
    printf("\n"); \
} while (0)

#define str_print(string) do { \
    String* __string_to_print = (string); \
    printf("%s", __string_to_print->slice); \
    del(String, __string_to_print); \
} while (0)

#define str_println(string) do { \
    String* __string_to_print = (string); \
    printf("%s", __string_to_print->slice); \
    printf("\n"); \
    del(String, __string_to_print); \
} while (0)

#define NEW_NULL_STRING (new(String, "<NULL>"))

// Static Methods

// Formats a string safely, without resulting in buffer overflows or other stupid behavior
String* str_format(char* format_string, ...);
String* str_create_with_length(uint32_t initial_length);

uint32_t str_slice_len(char* slice);

// Instance Methods

void str_append_char(String* string, char c);
void str_append_slice(String* string, char* text);
void str_append_string(String* string, String* other); // Consumes other

void str_pop(String* string, uint32_t amount);

bool str_in(String* string, uint32_t index);
char str_get_char(String* string, uint32_t index);
void str_set_char(String* string, uint32_t index, char c);
void str_swap_char(String* string, uint32_t left, uint32_t right);

bool str_equals_content(String* string, char* static_string);

// String Manipulations

// Removes all occurances of any of the characters in 'chars' in the string.
void str_remove_whitespace(String* string);
void str_remove_all(String* string, char* chars);

// Splits the string into substrings deliminated by (and removing) any of the characters in chars.
ArrayList* str_split_lines(String* string);
ArrayList* str_split_whitespace(String* string);
ArrayList* str_split_all(String* string, char* chars);

// Returns a new string which is a substring of this string
String* str_substring(String* string, uint32_t start_inclusive, uint32_t end_exclusive);

// Parsing
int32_t str_parse_int32(String* string); // Destructive

// Sorting
void str_sort(String* string);

// Private Methods

void str_ensure_length(String** string, uint32_t required_length); // Ensures a specific length of string will fit in this string, resizes otherwise

bool str_char_in_chars(char* chars, uint32_t length, char c); // Checks if a char is contained in a list of chars

bool str_qsort_lt_fn(String* string, uint32_t left, uint32_t right);
void str_qsort_swap_fn(String* string, uint32_t left, uint32_t right);

#endif