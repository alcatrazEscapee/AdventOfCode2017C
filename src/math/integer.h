// Classes: Int32, Int64
// Types: int32_t*, int64_t*
// Represents a boxed (heap allocated) integer

#ifndef INTEGER_H
#define INTEGER_H

#include "../class.h"

int32_t* constructor(Int32)(int32_t value);
DERIVE_CLASS_HEADER(Int32, int32_t*);

int64_t* constructor(Int64)(int64_t value);
DERIVE_CLASS_HEADER(Int64, int64_t*);

#endif