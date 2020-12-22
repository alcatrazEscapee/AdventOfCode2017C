// Defines a simple IntArrayList data structure

#ifndef INT_ARRAY_LIST_H
#define INT_ARRAY_LIST_H

#include "../aoc.h"

typedef struct {
    int32_t* values; // Backing array
    uint32_t size; // Length of backing array
    /* public readonly */ uint32_t length; // Size of filled elements
} IntArrayList;

// Constructor / Destructor
IntArrayList* IntArrayList__new(uint32_t initial_size);
void IntArrayList__del(IntArrayList* arl);

// Public Instance Methods - these all borrow the list

bool ial_in(IntArrayList* arl, uint32_t index); // Checks if a particular index is valid.

int32_t ial_get(IntArrayList* arl, uint32_t index); // Gets the element at index i, panics if the index is out of range.

void ial_set(IntArrayList* arl, uint32_t index, int32_t value); // Overwrites an element at a specific index. Panics if the index is out of range.
void ial_append(IntArrayList* arl, int32_t value); // Appends a new element to the list. Resizes the list if nessecary.

void ial_clear(IntArrayList* arl); // Removes all elements

#endif


