#include "strings.h"

// Private Methods

static void str_ensure_length(String string, uint32_t required_length); // Ensures a specific length of string will fit in this string, resizes otherwise

static bool str_char_in_chars(slice_t chars, char c); // Checks if a char is contained in a list of chars

static bool str_qsort_lt_fn(String string, uint32_t left, uint32_t right);
static void str_qsort_swap_fn(String string, uint32_t left, uint32_t right);

// Implement Class

impl_class(String);


// Static Methods - String Formatting

String str_format(slice_t format_string, ...)
{
    va_list arg;
    
    va_start(arg, format_string);
    uint32_t length = vsnprintf(NULL, 0, format_string, arg);
    va_end(arg);

    String string = str_create_with_length(length);

    va_start(arg, format_string);
    vsnprintf(string->slice, length + 1, format_string, arg);
    va_end(arg);

    string->length = length;
    return string;
}

String str_create_with_length(uint32_t initial_length)
{
    uint32_t initial_size = initial_length + 1; // Plus one for the null terminator
    String string = class_malloc(String);

    string->slice = safe_malloc(sizeof(char) * initial_size);
    string->size = initial_size;
    string->length = 0;

    return string;
}

// strlen is ungodly complicated for no good reason
uint32_t str_slice_len(slice_t slice)
{
    uint32_t length = 0;
    while (slice[length] != '\0')
    {
        length++;
    }
    return length;
}


String String__new(slice_t initial_value)
{
    uint32_t initial_length = str_slice_len(initial_value);
    uint32_t initial_size = initial_length + 1; // Plus one for the null terminator
    String string = class_malloc(String);

    string->slice = safe_malloc(sizeof(char) * initial_size);
    string->size = initial_length + 1;
    string->length = initial_length;

    memcpy(string->slice, initial_value, sizeof(char) * initial_size);

    return string;
}

void String__del(String string)
{
    free(string->slice);
    free(string);
}

String String__copy(String old)
{
    return new(String, old->slice);
}

bool String__equals(String left, String right)
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
int32_t String__compare(String left, String right)
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

uint32_t String__hash(String instance)
{
    uint32_t h = 0;
    for iter(String, it, instance)
    {
        h = (h * 31) + (uint32_t) it.value;
    }
    return h;
}

String String__format(String instance)
{
    // By specification of the Format trait, this must transfer ownership of the returned string to the caller
    // In addition, it must also borrow the provided string non-destructively.
    // Thus, in order to satisfy both constraints, we have to use copy() here, rather than returning the instance;
    return copy(String, instance);
}

bool StringSplit__iterator__test(Iterator(StringSplit)* it, String string, slice_t delim)
{
    del(String, it->value);
    if (it->index < string->length)
    {
        for (uint32_t next = it->index; next < string->length; next++)
        {
            char c = string->slice[next];
            if (str_char_in_chars(delim, c))
            {
                // Found the endpoint of the current slice
                if (next == it->index)
                {
                    // Empty string
                    it->value = new(String, "");
                    it->index++;
                }
                else
                {
                    // Non-empty string
                    it->value = str_substring(string, it->index, next);
                    it->index = next + 1;
                }
                return true;
            }
        }
        if (it->index < string->length)
        {
            // Last substring
            it->value = str_substring(string, it->index, string->length);
            it->index = string->length;
            return true;
        }
    }
    return false;
}


// Instance Methods

// Char + Bool can be simply optimized to direct slice insertions, integer types use str_append_format
void str_append_char(String string, char c)
{
    str_ensure_length(string, string->length + 1);
    string->slice[string->length] = c;
    string->slice[string->length + 1] = '\0';
    string->length += 1;
}

void str_append_bool(String string, bool b)
{
    if (b)
    {
        str_append_slice(string, "true");
    }
    else
    {
        str_append_slice(string, "false");
    }
}

// Other integer methods 
void str_append_int32_t(String string, int32_t i)
{
    str_append_format(string, "%d", i);
}

void str_append_int64_t(String string, int64_t i)
{
    str_append_format(string, "%ld", i);
}

void str_append_uint32_t(String string, uint32_t i)
{
    str_append_format(string, "%u", i);
}

void str_append_uint64_t(String string, uint64_t i)
{
    str_append_format(string, "%lu", i);
}

void str_append_slice(String string, slice_t text)
{
    uint32_t text_length = str_slice_len(text);
    str_ensure_length(string, string->length + text_length + 1);
    memcpy(&(string->slice[string->length]), text, sizeof(char) * text_length);
    string->slice[string->length + text_length] = '\0';
    string->length += text_length;
}

void str_append_string(String string, String other)
{
    str_ensure_length(string, string->length + other->length + 1);
    memcpy(&(string->slice[string->length]), other->slice, sizeof(char) * other->length);
    string->slice[string->length + other->length] = '\0';
    string->length += other->length;
    del(String, other);
}

void str_append_format(String string, slice_t format_string, ...)
{
    va_list arg;

    va_start(arg, format_string);
    uint32_t length = vsnprintf(NULL, 0, format_string, arg);
    va_end(arg);

    str_ensure_length(string, string->length + length);

    va_start(arg, format_string);
    vsnprintf((string->slice) + string->length, length + 1, format_string, arg);
    va_end(arg);

    string->length += length;
}

void str_pop(String string, uint32_t amount)
{
    panic_if(amount > string->length, "Tried to pop %d characters from the string '%s' which was only length %d at str_pop", amount, string->slice, string->length);
    string->length -= amount;
    string->slice[string->length] = '\0';
}

char str_get_char(String string, uint32_t index)
{
    panic_if(index >= string->length, "String index out of bounds: %d in {slice='%s', size=%d, length=%d} at str_get_char", index, string->slice, string->size, string->length);
    return string->slice[index];
}

void str_set_char(String string, uint32_t index, char c)
{
    panic_if(index >= string->length, "String index out of bounds: %d in {slice='%s', size=%d, length=%d} at str_set_char", index, string->slice, string->size, string->length);
    string->slice[index] = c;
}

bool str_equals_content(String string, slice_t static_string)
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

bool str_equals_slice(String string, uint32_t start_inclusive, uint32_t end_exclusive, slice_t static_string)
{
    panic_if(end_exclusive <= start_inclusive, "Total slice length must be positive: %d <= 0", end_exclusive - start_inclusive);
    panic_if(end_exclusive > string->length, "Slice must be within range of the string: end_exclusive = %d, and the string has length %d", end_exclusive, string->length);
    
    uint32_t slice_len = str_slice_len(static_string);
    panic_if(slice_len != end_exclusive - start_inclusive, "Slice [%d, %d) must be same length as length of static string (%d)", start_inclusive, end_exclusive, slice_len);

    for (uint32_t i = start_inclusive; i < end_exclusive; i++)
    {
        if (static_string[i - start_inclusive] != string->slice[i])
        {
            return false;
        }
    }
    return true;
}

// String Manipulations

Result(uint32_t) str_index_of(String string, slice_t any_chars)
{
    for iter(String, it, string)
    {
        if (str_char_in_chars(any_chars, it.value))
        {
            return Ok(uint32_t, it.index);
        }
    }
    return Err(uint32_t);
}

// Returns a new string which is a substring of this string
String str_substring(String string, uint32_t start_inclusive, uint32_t end_exclusive)
{
    panic_if(end_exclusive <= start_inclusive, "Cannot create a substring of length %d <= 0", end_exclusive - start_inclusive);
    panic_if(end_exclusive > string->length, "Cannot create a substring with end_exclusive = %d, as the string has length %d", end_exclusive, string->length);
    
    if (start_inclusive == 0 && end_exclusive == string->length)
    {
        // Special case, we're just copying the string
        return copy(String, string);
    }
    else
    {
        uint32_t length = end_exclusive - start_inclusive;
        String sub = str_create_with_length(length);
        memcpy(sub->slice, string->slice + start_inclusive, sizeof(char) * length);
        sub->slice[length] = '\0';
        sub->length = length;
        return sub;
    }
}

Result(char) str_parse_char(String string)
{
    return string->length > 0 ? Ok(char, string->slice[0]) : Err(char);
}

Result(bool) str_parse_bool(String string)
{
    if (string->length >= 4 && str_equals_slice(string, 0, 4, "true"))
    {
        return Ok(bool, true);
    }
    else if (string->length >= 5 && str_equals_slice(string, 0, 5, "false"))
    {
        return Ok(bool, false);
    }
    return Err(bool);
}

// Generic parsing methods declaration for all integer types
// This uses the default_format_string() macro defined for primitive types
#define impl_str_parse_simple(cls) \
Result(cls) CONCAT(str_parse_, cls) (String string) \
{ \
    cls e = default_value(cls); \
    int32_t ret = sscanf(string->slice, default_format_string(cls), &e); \
    return ret == 1 ? Ok(cls, e) : Err(cls); \
}

impl_str_parse_simple(int32_t);
impl_str_parse_simple(uint32_t);
impl_str_parse_simple(int64_t);
impl_str_parse_simple(uint64_t);


String str_escape(String string)
{
    String escaped_string = str_create_with_length(string->length);
    for iter(String, it, string)
    {
        switch (it.value)
        {
            case '\n':
                str_append_slice(escaped_string, "\\n");
                break;
            case '\t':
                str_append_slice(escaped_string, "\\t");
                break;
            case '\r':
                str_append_slice(escaped_string, "\\r");
                break;
            default:
                str_append_char(escaped_string, it.value);
                break;
        }
    }
    return escaped_string;
}

// Sorting

void str_sort(String string)
{
    if (string->length > 1)
    {
        // Invoke qsort, passing in the private methods as function pointers
        sorting_qsort_recursive(string, (FnSortingLessThan) &str_qsort_lt_fn, (FnSortingSwap) &str_qsort_swap_fn, 0, string->length - 1);
    }
}

// Private Methods

static void str_ensure_length(String string, uint32_t required_length)
{
    uint32_t required_size = required_length + 1;
    if (required_size > string->size)
    {
        // Resize the string
        string->size = required_size * 2; // Update the size of the string
        safe_realloc(string->slice, sizeof(char) * string->size); // Reallocate the slice

        // Initialize the rest of the new slice with nulls
        for (uint32_t i = string->length; i < string->size; i++)
        {
            string->slice[i] = '\0';
        }
    }
}

static bool str_char_in_chars(slice_t chars, char c)
{
    for (uint32_t i = 0; chars[i] != '\0'; i++)
    {
        if (c == chars[i])
        {
            return true;
        }
    }
    return false;
}

static bool str_qsort_lt_fn(String string, uint32_t left, uint32_t right)
{
    return string->slice[left] < string->slice[right];
}

static void str_qsort_swap_fn(String string, uint32_t left, uint32_t right)
{
    char c = string->slice[left];
    string->slice[left] = string->slice[right];
    string->slice[right] = c;
}

