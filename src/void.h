// This is a class definition for the type of NULL pointers.
// It allows a removal of annoying null-checking code (i.e. in ArrayHashSet), where class structs may be null
// Instead, all methods on this class implementation are designed to handle a null value, and expect it.

#ifndef VOID_H
#define VOID_H

#include <stdint.h> // int32_t, etc.
#include <stdbool.h> // true, false, bool

#include "class.h"
#include "strings.h"
#include "panic.h"

void* constructor(Void)();
DERIVE_CLASS_HEADER(Void, void*);

#endif