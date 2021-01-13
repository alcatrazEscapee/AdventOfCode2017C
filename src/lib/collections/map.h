// An Array backed Hash Map for generic key and value types
// Uses a simple backing array with linear probing for hash collisions

#include "../lib.h"

#ifndef COLLECTIONS_MAP_H
#define COLLECTIONS_MAP_H

struct Map__struct
{
    pointer_t* keys; // Key array
    pointer_t* values; // Value array
    Class key_class; // The key class
    Class value_class; // The value class
    uint32_t size; // The length of the backing array. Must be a power of 2
    uint32_t length; // The number of entries
};

typedef struct Map__struct * Map;

// This is a pseudo class
// It does not have a Class<T> object, nor implement all methods of the class
// However, it can still be used with new(), del(), and format()

declare_constructor(Map, uint32_t initial_size, Class key_class, Class value_class);

void Map__del(Map map);
String Map__format(Map map);

// Iterator
// test() is defined as a function in order to skip otherwise empty entries
// We still increment during next() (but do not assign key/value) in order to assign *after* the test passes, and avoid running off the end of the array.
// Additionally, test() is called before the first next(), resulting in needing some way to identify the first iteration a priori. 

typedef struct
{
    uint32_t index;
    void* key;
    void* value;
} Iterator(Map);

bool Map__iterator__test(Iterator(Map)* it, Map map);

#define Map__iterator__start(map) { 0, NULL, NULL }
#define Map__iterator__next(it, map) (it)->index++


// Public Instance Methods - these all borrow the map

// Puts a (key, value) pair into the map. Returns true if the key was already in the map
bool map_put(Map map, pointer_t key, pointer_t value);

// Checks if a key is present in the map.
bool map_contains_key(Map map, pointer_t key);

// Gets the current value associated to a particular key, or NULL if there was none
pointer_t map_get(Map map, pointer_t key);

// Clears the map
void map_clear(Map map);

#endif