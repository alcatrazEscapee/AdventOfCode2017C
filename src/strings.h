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
#include "utils.h"

// Class Definitions
String* constructor(String)(char* initial_value);
DERIVE_CLASS_HEADER(String, String*);

// Iterator
#define String__iter(string, index, character) uint32_t index = 0; for (char character = '\0'; index < string->length ? (character = string->slice[index], true) : false; index++)

// Static Methods

// Formats a string safely, without resulting in buffer overflows or other stupid behavior
String* str_format(char* format_string, ...);
String* str_create_with_length(uint32_t initial_length);

uint32_t str_slice_len(char* slice);

// Instance Methods

void str_append_char(String* string, char c);
void str_append_slice(String* string, char* text);
void str_append_string(String* string, String* other); // Consumes other

bool str_in(String* string, uint32_t index);
char str_get_char(String* string, uint32_t index);
void str_set_char(String* string, uint32_t index, char c);

bool str_equals_content(String* string, char* static_string);

// Private Methods

void str_ensure_length(String** string, uint32_t required_length); // Ensures a specific length of string will fit in this string, resizes otherwise

#endif