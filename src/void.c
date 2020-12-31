#include "void.h"

DERIVE_CLASS(Void);

void* Void__new()
{
    PANIC("Invoked new(Void)... why?");
}

void Void__del(void* v)
{
    // No-op
}

void* Void__copy(void* v)
{
    return NULL;
}

bool Void__equals(void* left, void* right)
{
    return true;
}

int32_t Void__compare(void* left, void* right)
{
    return 0;
}

uint32_t Void__hash(void* v)
{
    return 0;
}

String* Void__format(void* v)
{
    return NULL_STRING;
}