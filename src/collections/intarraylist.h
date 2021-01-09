// An ArrayList<int32_t>
// Implemented as a class and can be used as a Tuple<int32_t>
// Automatically resizable - is initialized with exactly the original size

#include "../lib.h"

#ifndef INT_ARRAY_LIST_H
#define INT_ARRAY_LIST_H

typedef struct
{
    int32_t* values; // Backing array
    uint32_t size; // Length of backing array
    /* public readonly */ uint32_t length; // Size of filled elements
} IntArrayList;

// Class
IntArrayList* IntArrayList__new(uint32_t initial_size);
DERIVE_CLASS_HEADER(IntArrayList, IntArrayList*);

// Iterator
typedef struct
{
    uint32_t index;
    int32_t value;
} Iterator(IntArrayList);

#define IntArrayList__iterator__start(list) { 0, 0 }
#define IntArrayList__iterator__test(it, list) (it)->index < (list)->length ? (it)->value = (list)->values[(it)->index], true : false
#define IntArrayList__iterator__next(it, list) (it)->index ++

// Static Methods

IntArrayList* ial_create_from_array(int32_t* array, uint32_t size);

// Public Instance Methods - these all borrow the list

bool ial_in(IntArrayList* arl, uint32_t index); // Checks if a particular index is valid.

int32_t ial_get(IntArrayList* arl, uint32_t index); // Gets the element at index i, panics if the index is out of range.

void ial_set(IntArrayList* arl, uint32_t index, int32_t value); // Overwrites an element at a specific index. Panics if the index is out of range.
void ial_append(IntArrayList* arl, int32_t value); // Appends a new element to the list. Resizes the list if nessecary.

void ial_clear(IntArrayList* arl); // Removes all elements

#endif


