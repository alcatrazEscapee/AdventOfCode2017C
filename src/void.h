// This is a class definition for the type of NULL pointers.
// It allows a removal of annoying null-checking code (i.e. in ArrayHashSet), where class structs may be null
// Instead, all methods on this class implementation are designed to handle a null value, and expect it.

#include "common.h"

#ifndef VOID_H
#define VOID_H

void* Void__new();
DERIVE_CLASS_HEADER(Void, void*);

#endif