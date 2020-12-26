// Defines a generic ArrayList data structure

#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#include <stdio.h> // printf, etc.
#include <stdlib.h> // malloc, free
#include <stdint.h> // int32_t, etc.
#include <stdbool.h> // true, false, bool
#include <string.h> // memcpy

struct ArrayList__struct;

#include "../class.h"

typedef struct ArrayList__struct {
    void** values; // Backing array
    Class* value_class; // Values class
    uint32_t size; // Length of backing array
    /* public readonly */ uint32_t length; // Size of filled elements
} ArrayList;

#include "../utils.h"
#include "../panic.h"
#include "../strings.h"


// Constructor / Destructor
ArrayList* ArrayList__new(uint32_t initial_size, Class* value_class);
void ArrayList__del(ArrayList* arl);

String* ArrayList__format(ArrayList* arl);

// Iterator
// Leaks the value outside the iterator scope
#define ArrayList__iter(type, array, i, v) type v = 0; for (uint32_t i = 0; (i < (array)->length ? (v = (type) (array)->values[i]), true : false); i++)

// Public Instance Methods - these all borrow the list

bool al_in(ArrayList* arl, uint32_t index); // Checks if a particular index is valid.

void* al_get(ArrayList* arl, uint32_t index); // Gets the element at index i, panics if the index is out of range.

void al_set(ArrayList* arl, uint32_t index, void* value); // Overwrites an element at a specific index. Panics if the index is out of range.
void al_append(ArrayList* arl, void* value); // Appends a new element to the list. Resizes the list if nessecary.

void al_clear(ArrayList* arl); // Removes all elements

#endif


