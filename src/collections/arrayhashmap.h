// An array backed hash map
// Stores key value pairs with O(1) acccess.
// Generically typed

#ifndef ARRAY_HASH_MAP_H
#define ARRAY_HASH_MAP_H

#include <stdio.h> // printf, etc.
#include <stdlib.h> // malloc, free
#include <stdint.h> // int32_t, etc.
#include <stdbool.h> // true, false, bool
#include <string.h> // memcpy

struct ArrayHashMapEntry__struct;
struct ArrayHashMap__struct;

#include "../class.h"
#include "../utils.h"
#include "../panic.h"

typedef struct ArrayHashMapEntry__struct {
    uint32_t index;
    struct ArrayHashMapEntry__struct* next;
    struct ArrayHashMap__struct* map;
    /* public readonly */ void* key;
    /* public readonly */ void* value;
} ArrayHashMapEntry;

typedef struct ArrayHashMap__struct {
    struct ArrayHashMapEntry__struct ** array; // Backing array
    uint32_t size; // The length of the backing array. Must be a power of 2
    Class* key_class; // The key class
    Class* value_class; // The value class
    /* public readonly */ uint32_t length; // The number of entries
} ArrayHashMap;


// Constructor / Destructor
ArrayHashMapEntry* ArrayHashMapEntry__new(ArrayHashMap* map, void* key, void* value);
void ArrayHashMapEntry__del(ArrayHashMapEntry* entry);

#define LOAD_FACTOR 0.75
#define PRIME 31

// Constructor / Destructor
ArrayHashMap* ArrayHashMap__new(uint32_t initial_size, Class* key_class, Class* value_class);
void ArrayHashMap__del(ArrayHashMap* map);

// Public Instance Methods - these all borrow the map

// Puts a (key, value) pair into the map. Returns the current value at that key, or NULL if there was none
void* ahm_put(ArrayHashMap* map, void* key, void* value);

// Gets the current value associated to a particular key, or NULL if there was none
void* ahm_get(ArrayHashMap* map, void* key);

void ahm_print(ArrayHashMap* map, void (*entry_writer)(ArrayHashMapEntry*));

// Iterator
#define ArrayHashMap__iter(map, entry) for (ArrayHashMapEntry* entry = NULL; ahm_iter_next_entry(map, &entry), entry != NULL;)

void ahm_iter_next_entry(ArrayHashMap* map, ArrayHashMapEntry** entry);

#endif