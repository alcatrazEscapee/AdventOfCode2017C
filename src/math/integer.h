// Classes: Int32, Int64
// Types: int32_t*, int64_t*
// Represents a boxed (heap allocated) integer

#include "../lib.h"

#ifndef INTEGER_H
#define INTEGER_H

int32_t* Int32__new(int32_t value);
DERIVE_CLASS_HEADER(Int32, int32_t*);

int64_t* Int64__new(int64_t value);
DERIVE_CLASS_HEADER(Int64, int64_t*);

#endif