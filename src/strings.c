#include "strings.h"

// String-Adjacent Methods

void println(char* format_string, ...)
{
    va_list arg;
    va_start(arg, format_string);
    vprintf(format_string, arg);
    va_end(arg);
    printf("\n");
}

void str_print(String* string)
{
    printf("%s", string->slice);
    del(String, string);
}

void str_println(String* string)
{
    printf("%s\n", string->slice);
    del(String, string);
}

// Static Methods - String Formatting

String* str_format(char* format_string, ...)
{
    va_list arg;
    
    va_start(arg, format_string);
    uint32_t length = vsnprintf(NULL, 0, format_string, arg);
    va_end(arg);

    String* string = str_create_with_length(length);

    va_start(arg, format_string);
    vsnprintf(string->slice, length + 1, format_string, arg);
    va_end(arg);

    string->length = length;
    return string;
}

String* str_create_with_length(uint32_t initial_length)
{
    uint32_t initial_size = next_highest_power_of_two(initial_length + 1); // Plus one for the null terminator
    String* string = (String*) malloc(sizeof(String));
    char* slice = (char*) malloc(sizeof(char) * initial_size);

    PANIC_IF_NULL(string, "Unable to create string with size %d, length %d at str_create_with_length(uint32_t)", initial_size, initial_length);
    PANIC_IF_NULL(slice, "Unable to create string backing array with size %d, length %d at str_create_with_length(uint32_t)", initial_size, initial_length);

    string->slice = slice;
    string->size = initial_size;
    string->length = 0;

    return string;
}

// strlen is ungodly complicated for no good reason
uint32_t str_slice_len(char* slice)
{
    uint32_t length = 0;
    while (slice[length] != '\0')
    {
        length++;
    }
    return length;
}


DERIVE_CLASS(String); // Class

String* String__new(char* initial_value) // Constructor
{
    uint32_t initial_length = str_slice_len(initial_value);
    uint32_t initial_size = next_highest_power_of_two(initial_length + 1); // Plus one for the null terminator
    String* string = (String*) malloc(sizeof(String));
    char* slice = (char*) malloc(sizeof(char) * initial_size);

    PANIC_IF_NULL(string, "Unable to create string from %s with size %d, length %d", initial_value, initial_size, initial_length);
    PANIC_IF_NULL(slice, "Unable to create string backing array from %s with size %d, length %d", initial_value, initial_size, initial_length);

    string->slice = slice;
    string->size = initial_size;
    string->length = initial_length;

    memcpy(string->slice, initial_value, sizeof(char) * initial_length);
    for (uint32_t i = initial_length; i < initial_size; i++)
    {
        string->slice[i] = '\0'; // Initialize all remaining length to null
    }
    return string;
}

void String__del(String* string) // Destructor
{
    free(string->slice);
    free(string);
}

String* String__copy(String* old) // Copy
{
    return new(String, old->slice);
}

bool String__equals(String* left, String* right) // Equals
{
    if (left->length != right->length)
    {
        return false;
    }
    uint32_t length = left->length;
    for (uint32_t i = 0; i < length; i++)
    {
        if (left->slice[i] != right->slice[i])
        {
            return false;
        }
    }
    return true;
}

// Represents an ASCII code ordering
int32_t String__compare(String* left, String* right) // Compare
{
    uint32_t length = min(left->length, right->length);
    for (uint32_t i = 0; i < length; i++)
    {
        if (left->slice[i] < right->slice[i])
        {
            return -1;
        }
        else if (left->slice[i] > right->slice[i])
        {
            return 1;
        }
    }
    // Longer strings are bigger
    if (left->length > right->length)
    {
        return 1;
    }
    else if (left->length < right->length)
    {
        return -1;
    }
    else
    {
        return 0; // Strings must be equal
    }
}

uint32_t String__hash(String* instance) // Hash
{
    uint32_t h = 0;
    for iter(String, it, instance)
    {
        h = (h * 31) + (uint32_t) it.value;
    }
    return h;
}

String* String__format(String* instance) // Format
{
    // By specification of the Format trait, this must transfer ownership of the returned string to the caller
    // In addition, it must also borrow the provided string non-destructively.
    // Thus, in order to satisfy both constraints, we have to use copy() here, rather than returning the instance;
    return copy(String, instance);
}

// Instance Methods

void str_append_char(String* string, char c)
{
    str_ensure_length(&string, string->length + 1);
    string->slice[string->length] = c;
    string->slice[string->length + 1] = '\0';
    string->length += 1;
}

void str_append_slice(String* string, char* text)
{
    uint32_t text_length = str_slice_len(text);
    str_ensure_length(&string, string->length + text_length + 1);
    memcpy(&(string->slice[string->length]), text, sizeof(char) * text_length);
    string->slice[string->length + text_length] = '\0';
    string->length += text_length;
}

void str_append_string(String* string, String* other)
{
    str_ensure_length(&string, string->length + other->length + 1);
    memcpy(&(string->slice[string->length]), other->slice, sizeof(char) * other->length);
    string->slice[string->length + other->length] = '\0';
    string->length += other->length;
    del(String, other);
}

void str_pop(String* string, uint32_t amount)
{
    if (amount > string->length) PANIC("Tried to pop %d characters from the string '%s' which was only length %d", amount, string->slice, string->length);
    string->length -= amount;
    string->slice[string->length] = '\0';
}

bool str_in(String* string, uint32_t index)
{
    return index < string->length;
}

char str_get_char(String* string, uint32_t index)
{
    if (!str_in(string, index)) PANIC("String index out of bounds: %d in {slice='%s', size=%d, length=%d} at str_get_char(String*, uint32_t)", index, string->slice, string->size, string->length);
    return string->slice[index];
}

void str_set_char(String* string, uint32_t index, char c)
{
    if (!str_in(string, index)) PANIC("String index out of bounds: %d in {slice='%s', size=%d, length=%d} at str_set_char(String*, uint32_t)", index, string->slice, string->size, string->length);
    string->slice[index] = c;
}

bool str_equals_content(String* string, char* static_string)
{
    for iter(String, it, string)
    {
        // Reached the end of the static string before string, OR
        // Not equal at position i
        if (static_string[it.index] == '\0' || it.value != static_string[it.index])
        {
            return false;
        }
    }
    // Reached the end of string. Check if static_string has also ended
    if (static_string[string->length] == '\0')
    {
        return true;
    }
    return false;
}

// String Manipulations

void str_remove_whitespace(String* string)
{
    return str_remove_all(string, "\r\n\t ");
}

void str_remove_all(String* string, char* chars)
{
    uint32_t chars_length = str_slice_len(chars);
    uint32_t removed = 0;
    for (uint32_t pointer = 0; pointer < string->length; pointer++)
    {
        if (str_char_in_chars(chars, chars_length, string->slice[pointer]))
        {
            removed++;
        }
        else
        {
            string->slice[pointer - removed] = string->slice[pointer];
        }
    }
    string->length -= removed;
}

ArrayList* str_split_lines(String* string)
{
    return str_split_all(string, "\r\n");
}

ArrayList* str_split_whitespace(String* string)
{
    return str_split_all(string, "\r\n\t ");
}

// Splits the string into substrings deliminated by (and removing) any of the characters in chars.
ArrayList* str_split_all(String* string, char* chars)
{
    ArrayList* list = new(ArrayList, 10, class(String));
    uint32_t chars_length = str_slice_len(chars);
    uint32_t last = 0;
    for (uint32_t pointer = 0; pointer < string->length; pointer++)
    {
        if (str_char_in_chars(chars, chars_length, string->slice[pointer]))
        {
            // Create and add a new substring, if the previous length is nonzero
            if (pointer > last)
            {
                al_append(list, str_substring(string, last, pointer));
            }
            last = pointer + 1;
        }
    }
    // Include the last substring
    if (string->length > last)
    {
        al_append(list, str_substring(string, last, string->length));
    }
    return list;
}

// Returns a new string which is a substring of this string
String* str_substring(String* string, uint32_t start_inclusive, uint32_t end_exclusive)
{
    if (end_exclusive <= start_inclusive) PANIC("Cannot create a substring of length %d <= 0", end_exclusive - start_inclusive);
    if (end_exclusive > string->length) PANIC("Cannot create a substring with end_exclusive = %d, as the string has length %d", end_exclusive, string->length);
    
    if (start_inclusive == 0 && end_exclusive == string->length)
    {
        // Special case, we're just copying the string
        return copy(String, string);
    }
    else
    {
        uint32_t length = end_exclusive - start_inclusive;
        String* sub = str_create_with_length(length);
        memcpy(sub->slice, string->slice + start_inclusive, sizeof(char) * length);
        sub->slice[length] = '\0';
        sub->length = length;
        return sub;
    }
}

int32_t str_parse_int32(String* string)
{
    int32_t e = 0;
    sscanf(string->slice, "%d", &e);
    return e;
}

// Sorting

void str_sort(String* string)
{
    if (string->length > 1)
    {
        sorting_qsort_recursive(string, (FnSortingLessThan) &str_qsort_lt_fn, (FnSortingSwap) &str_qsort_swap_fn, 0, string->length - 1);
    }
}

// Private Methods

void str_ensure_length(String** string, uint32_t required_length)
{
    uint32_t required_size = required_length + 1;
    if (required_size > (*string)->size)
    {
        // Resize the string
        uint32_t new_size = next_highest_power_of_two(required_size);
        char* new_slice = (char*) malloc(sizeof(char) * new_size);

        PANIC_IF_NULL(new_slice, "Unable to extend string from length %d / size %d to length %d / size %d", (*string)->length, (*string)->size, required_length, new_size);

        memcpy(new_slice, (*string)->slice, sizeof(char) * (*string)->length); // Copy the existing string to the new slice
        free((*string)->slice); // Free the old slice
        (*string)->slice = new_slice; // Point the string at the new slice
        (*string)->size = new_size; // Update the size of the string

        // Initialize the rest of the new slice with nulls
        for (uint32_t i = (*string)->length; i < (*string)->size; i++)
        {
            (*string)->slice[i] = '\0';
        }
    }
}

bool str_char_in_chars(char* chars, uint32_t length, char c)
{
    for (uint32_t i = 0; i < length; i++)
    {
        if (c == chars[i])
        {
            return true;
        }
    }
    return false;
}

bool str_qsort_lt_fn(String* string, uint32_t left, uint32_t right)
{
    return string->slice[left] < string->slice[right];
}

void str_qsort_swap_fn(String* string, uint32_t left, uint32_t right)
{
    char c = string->slice[left];
    string->slice[left] = string->slice[right];
    string->slice[right] = c;
}

