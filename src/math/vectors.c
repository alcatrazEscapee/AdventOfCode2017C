#include "vectors.h"

// Vec2i Class Implementation

DERIVE_CLASS(Vec2i);

Vec2i* constructor(Vec2i)(int32_t x, int32_t y)
{
    Vec2i* p = malloc(sizeof(Vec2i));
    PANIC_IF_NULL(p, "Unable to create Vec2i (%d, %d)", x, y);
    p->x = x;
    p->y = y;
    return p;
}

DERIVE_CLASS_DEL(Vec2i, Vec2i*);
DERIVE_CLASS_COMPARE_PANIC(Vec2i, Vec2i*);

Vec2i* Vec2i__copy(Vec2i* p)
{
    return new(Vec2i, p->x, p->y);
}

bool Vec2i__equals(Vec2i* p, Vec2i* q)
{
    return (p->x == q->x) && (p->y == q->y);
}

uint32_t Vec2i__hash(Vec2i* p)
{
    return ((uint32_t) p->x) ^ ((uint32_t) (27644437 * p->y));
}

String* Vec2i__format(Vec2i* p)
{
    return str_format("Vec2i(%d, %d)", p->x, p->y);
}

// Instance Methods

void v2i_set(Vec2i* p, int32_t x, int32_t y)
{
    p->x = x;
    p->y = y;
}