// A heap allocated String data type with utility methods, similar to Rust's String vs. &'static str
// Unlike Rust's string natively handling unicode data, we make the (simplifying) assumptions that a character represents a single byte of 7-bit ASCII data.

#include "lib.h"
#include "collections/arraylist.h"

#ifndef STRINGS_H
#define STRINGS_H

// Class Definitions
String* String__new(char* initial_value);
DERIVE_CLASS_HEADER(String, String*);

// Iterator
// This iterator is the default iter(String) iterator, and will evaluate for each character in the string
typedef struct
{
    uint32_t index;
    char value;
} Iterator(String);

#define String__iterator__start(string) { 0, '\0' }
#define String__iterator__test(it, string) (it)->index < (string)->length ? (((it)->value = (string)->slice[(it)->index]), true) : false
#define String__iterator__next(it, string) (it)->index++

// StringSplit iterators
// Splits a string into substrings according to delimiter characters
// The delimiters must be a static string as it's inlined directly into method calls
// Usage:
// for iter(StringSplit, it, string, "\n\r") {
//     it.value, a String* which is mutably borrowed   
// }

typedef struct
{
    String* value;
    uint32_t index;
} Iterator(StringSplit);

#define StringSplit__iterator__start(string, delim) { NULL, 0 }
#define StringSplit__iterator__next(it, string, delim) (void)0

bool StringSplit__iterator__test(Iterator(StringSplit)* it, String* string, char* delim);


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
void str_set_char(String* string, uint32_t index, char c); // Sets the char at a given index

bool str_equals_content(String* string, char* static_string);

// String Manipulations

// Returns a new string which is a substring of this string
String* str_substring(String* string, uint32_t start_inclusive, uint32_t end_exclusive);

// Parsing
int32_t str_parse_int32(String* string);
String* str_escape(String* string); // Converts a string to escaped form (e.g. '\t\r\n' -> '\\t\\r\\n')

// Sorting
void str_sort(String* string);

// Private Methods

void str_ensure_length(String** string, uint32_t required_length); // Ensures a specific length of string will fit in this string, resizes otherwise

bool str_char_in_chars(char* chars, char c); // Checks if a char is contained in a list of chars

bool str_qsort_lt_fn(String* string, uint32_t left, uint32_t right);
void str_qsort_swap_fn(String* string, uint32_t left, uint32_t right);

#endif