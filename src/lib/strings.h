// A heap allocated String data type with utility methods, similar to Rust's String vs. &'static str
// Unlike Rust's string natively handling unicode data, we make the (simplifying) assumptions that a character represents a single byte of 7-bit ASCII data.

#include "lib.h"

#ifndef STRINGS_H
#define STRINGS_H

// Class Definitions
declare_class(String);
declare_constructor(String, slice_t initial_value);

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
    String value;
    uint32_t index;
} Iterator(StringSplit);

#define StringSplit__iterator__start(string, delim) { NULL, 0 }
#define StringSplit__iterator__next(it, string, delim) (void)0

bool StringSplit__iterator__test(Iterator(StringSplit)* it, String string, slice_t delim);

// String-Adjacent Methods

void println(slice_t format_string, ...); // printf, but with a '\n' appended to the output

void str_print(String string); // Destructive printing of a string. For non-destructive behavior, write printf("%s", string->slice);
void str_println(String string); // Destructive printing of a string. For non-destructive behavor, write println("%s\n", string->slice);

// Static Methods

String str_format(slice_t format_string, ...); // Formats using printf-style formatting to an output string
String str_create_with_length(uint32_t initial_length); // Creates an empty string with at least the specified length

uint32_t str_slice_len(slice_t slice);

// Instance Methods

// Generic specialization
#define str_append(string, value) _Generic((value), \
    char: str_append_char, \
    bool: str_append_bool, \
    int32_t: str_append_int32_t, \
    int64_t: str_append_int64_t, \
    uint32_t: str_append_uint32_t, \
    uint64_t: str_append_uint64_t, \
    slice_t: str_append_slice, \
    String: str_append_string \
) (string, value)

// Primitive Type Methods
void str_append_char(String string, char c);
void str_append_bool(String string, bool b);
void str_append_int32_t(String string, int32_t i);
void str_append_int64_t(String string, int64_t i);
void str_append_uint32_t(String string, uint32_t i);
void str_append_uint64_t(String string, uint64_t i);

void str_append_slice(String string, slice_t text);
void str_append_string(String string, String other); // Consumes other

void str_append_format(String string, slice_t format_string, ...); // Formats using printf-style formatting, appending to an existing string

void str_pop(String string, uint32_t amount);

char str_get_char(String string, uint32_t index);
void str_set_char(String string, uint32_t index, char c); // Sets the char at a given index

bool str_equals_content(String string, slice_t static_string);

// String Manipulations

// Returns a new string which is a substring of this string
String str_substring(String string, uint32_t start_inclusive, uint32_t end_exclusive);

// Parsing
int32_t str_parse_int32(String string);
String str_escape(String string); // Converts a string to escaped form (e.g. '\t\r\n' -> '\\t\\r\\n')

// Sorting
void str_sort(String string);

#endif