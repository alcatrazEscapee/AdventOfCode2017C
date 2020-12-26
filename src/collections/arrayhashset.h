// An array backed hash set
// Has O(1) contains checks (ahs_in)
// Implemented using an ArrayHashMap with NULL values

#ifndef ARRAY_HASH_SET_H
#define ARRAY_HASH_SET_H

#include <stdio.h> // printf, etc.
#include <stdlib.h> // malloc, free
#include <stdint.h> // int32_t, etc.
#include <stdbool.h> // true, false, bool
#include <string.h> // memcpy

#include "../class.h"
#include "../utils.h"
#include "../panic.h"
#include "../void.h"

#include "arrayhashmap.h"

typedef struct ArrayHashMap__struct ArrayHashSet;

// Constructor / Destructor
ArrayHashSet* ArrayHashSet__new(uint32_t initial_size, Class* key_class);
void ArrayHashSet__del(ArrayHashSet* set);

String* ArrayHashSet__format(ArrayHashSet* set);

// Public Instance Methods - these all borrow the map

// Puts a key into the set, returns true if it was already in the set
bool ahs_put(ArrayHashSet* set, void* key);

// Checks if a key is in the set
bool ahs_in(ArrayHashSet* set, void* key);

void ahs_clear(ArrayHashSet* set);

// Iterator
#define ArrayHashSet__iter(set, type, entry, value) type value = NULL; for (ArrayHashMapEntry* entry = NULL; ahm_iter_next_entry((set)->map, &entry), value = entry->value, entry != NULL;)

#endif