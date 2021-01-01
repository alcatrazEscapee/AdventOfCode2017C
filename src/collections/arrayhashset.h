// An array backed hash set
// Has O(1) contains checks (ahs_in)
// Implemented using an ArrayHashMap with NULL values

#include "../common.h"
#include "arrayhashmap.h"

#ifndef ARRAY_HASH_SET_H
#define ARRAY_HASH_SET_H

// A set is just a map in disguise
typedef ArrayHashMap ArrayHashSet;
typedef Iterator(ArrayHashMap) Iterator(ArrayHashSet);

// Constructor / Destructor
ArrayHashSet* ArrayHashSet__new(uint32_t initial_size, Class* key_class);
void ArrayHashSet__del(ArrayHashSet* set);

String* ArrayHashSet__format(ArrayHashSet* set);

// Iterator

#define ArrayHashSet__iterator__start(map) ArrayHashMap__iterator__start(map)
#define ArrayHashSet__iterator__test(it, map) ArrayHashMap__iterator__test(it, map)
#define ArrayHashSet__iterator__next(it, map) ArrayHashMap__iterator__next(it, map)


// Public Instance Methods - these all borrow the map

// Puts a key into the set, returns true if it was already in the set
bool ahs_put(ArrayHashSet* set, void* key);

// Checks if a key is in the set
bool ahs_in(ArrayHashSet* set, void* key);

void ahs_clear(ArrayHashSet* set);

#endif