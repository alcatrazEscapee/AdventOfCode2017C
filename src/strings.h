// A heap allocated String data type with utility methods, similar to Rust's String vs. &'static str
// Unlike Rust's string natively handling unicode data, we make the (simplifying) assumptions that a character represents a single byte of 7-bit ASCII data.

#include "common.h"
#include "collections/arraylist.h"

#ifndef STRINGS_H
#define STRINGS_H

// Class Definitions
String* String__new(char* initial_value);
DERIVE_CLASS_HEADER(String, String*);

// Iterator
typedef struct
{
    uint32_t index;
    char value;
} Iterator(String);

#define String__iterator__start(string) { 0, '\0' }
#define String__iterator__test(it, string) (it)->index < (string)->length ? ((it)->value = (string)->slice[(it)->index], true) : false
#define String__iterator__next(it, string) (it)->index++

#define NULL_STRING (new(String, "<NULL>"))
#define EMPTY_STRING (new(String, ""))


// String-Adjacent Methods

void println(char* format_string, ...); // printf, but with a '\n' appended to the output

void str_print(String* string); // Destructive printing of a string. For non-destructive behavior, write printf("%s", string->slice);
void str_println(String* string); // Destructive printing of a string. For non-destructive behavor, write println("%s\n", string->slice);

// Static Methods

String* str_format(char* format_string, ...); // Formats using printf-style formatting to an output string
String* str_create_with_length(uint32_t initial_length); // Creates an empty string with at least the specified length

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