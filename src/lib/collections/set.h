// An array backed hash set
// Implementation is a subset of the Map implementation, using just the keys array
// Cannot contain NULL values

#include "../lib.h"

#ifndef COLLECTIONS_SET_H
#define COLLECTIONS_SET_H

struct Set__struct
{
    pointer_t* values; // Value array
    Class value_class; // The value class
    uint32_t size; // The length of the backing array. Must be a power of 2
    uint32_t length; // The number of entries
};

typedef struct Set__struct * Set;

// This is a pseudo class
// It does not have a Class<T> object, nor implement all methods of the class
// However, it can still be used with new(), del(), and format()

declare_constructor(Set, uint32_t initial_size, Class value_class);

void Set__del(Set set);
String Set__format(Set set);

// Iterator
// test() is defined as a function in order to skip otherwise empty entries
// We still increment during next() (but do not assign key/value) in order to assign *after* the test passes, and avoid running off the end of the array.
// Additionally, test() is called before the first next(), resulting in needing some way to identify the first iteration a priori. 

typedef struct
{
    uint32_t index;
    pointer_t value;
} Iterator(Set);

bool Set__iterator__test(Iterator(Set)* it, Set set);

#define Set__iterator__start(set) { 0, NULL }
#define Set__iterator__next(it, set) (it)->index++


// Public Instance Methods - these all borrow the set

// Puts an value into the set. Returns true if the value was already in the set
bool set_put(Set set, pointer_t value);

// Checks if a value is present in the set.
bool set_contains(Set set, pointer_t value);

// Clears the set
void set_clear(Set set);

#endif