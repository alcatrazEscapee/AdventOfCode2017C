#include "integer.h"

// Int32 / int32_t*

DERIVE_CLASS(Int32); // Class
DERIVE_CLASS_IMPL_ALL(Int32, int32_t*); // Implement Equals, Compare, Copy, Del

int32_t* constructor(Int32)(int32_t value) // Constructor
{
    int32_t* p = malloc(sizeof(int32_t));
    PANIC_IF_NULL(p, "Unable to create Int32");
    *p = value;
    return p;
}

uint32_t Int32__hash(int32_t* p) // Hash
{
    return (uint32_t) *p;
}

String* Int32__format(int32_t* p) // Format
{
    return str_format("%d", *p);
}

// Int64 / int64_t*

DERIVE_CLASS(Int64); // Class
DERIVE_CLASS_IMPL_ALL(Int64, int64_t*);

int64_t* constructor(Int64)(int64_t value)
{
    int64_t* p = malloc(sizeof(int64_t));
    PANIC_IF_NULL(p, "Unable to create Int64");
    *p = value;
    return p;
}

uint32_t Int64__hash(int64_t* p) // Hash
{
    return (uint32_t) ((*p >> 32) ^ *p);
}

String* Int64__format(int64_t* p) // Format
{
    return str_format("%l", *p);
}
