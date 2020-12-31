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

Iterator(String)* String__iterator__new(String* string);
void String__iterator__del(Iterator(String)* it);

#define String__iterator__test(it, string) (it)->index < (string)->length ? ((it)->value = (string)->slice[(it)->index], true) : false
#define String__iterator__next(it, string) (it)->index++


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