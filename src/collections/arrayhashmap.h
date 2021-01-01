// An Array backed Hash Map for generic key and value types
// Uses a simple backing array with linear probing for hash collisions

#include "../common.h"

#ifndef ARRAY_HASH_MAP_H
#define ARRAY_HASH_MAP_H

typedef struct
{
    void** keys; // Key array
    void** values; // Value array
    Class* key_class; // The key class
    Class* value_class; // The value class
    uint32_t size; // The length of the backing array. Must be a power of 2
    /* public readonly */ uint32_t length; // The number of entries
} ArrayHashMap;


#define LOAD_FACTOR 0.75

// Constructor / Destructor
ArrayHashMap* ArrayHashMap__new(uint32_t initial_size, Class* key_class, Class* value_class);
void ArrayHashMap__del(ArrayHashMap* map);

String* ArrayHashMap__format(ArrayHashMap* map);

// Iterator
// test() is defined as a function in order to skip otherwise empty entries
// We still increment during next() (but do not assign key/value) in order to assign *after* the test passes, and avoid running off the end of the array.
// Additionally, test() is called before the first next(), resulting in needing some way to identify the first iteration a priori. 

typedef struct
{
    uint32_t index;
    void* key;
    void* value;
} Iterator(ArrayHashMap);

bool ArrayHashMap__iterator__test(Iterator(ArrayHashMap)* it, ArrayHashMap* map);

#define ArrayHashMap__iterator__start(map) { 0, NULL, NULL }
#define ArrayHashMap__iterator__next(it, map) (it)->index++


// Public Instance Methods - these all borrow the map

// Puts a (key, value) pair into the map. Returns true if the key was already in the map
bool ahm_put(ArrayHashMap* map, void* key, void* value);

// Checks if a key is present in the map.
bool ahm_key_in(ArrayHashMap* map, void* key);

// Gets the current value associated to a particular key, or NULL if there was none
void* ahm_get(ArrayHashMap* map, void* key);

// Clears the map
void ahm_clear(ArrayHashMap* map);


// Private Methods

void ahm_rehash(ArrayHashMap* map);

#endif