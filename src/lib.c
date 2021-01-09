
#include "lib.h"

// Move Semantics
void* __move(void** ref_ptr)
{
    void* moved_ptr = *ref_ptr;
    *ref_ptr = NULL;
    return moved_ptr;
}