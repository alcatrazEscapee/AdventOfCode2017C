// Defines a generic ArrayList data structure

#include "../common.h"

#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

struct ArrayList__struct;

typedef struct ArrayList__struct
{
    void** values; // Backing array
    Class* value_class; // Values class
    uint32_t size; // Length of backing array
    /* public readonly */ uint32_t length; // Size of filled elements
} ArrayList;

// Constructor / Destructor
ArrayList* ArrayList__new(uint32_t initial_size, Class* value_class);
void ArrayList__del(ArrayList* arl);
ArrayList* ArrayList__copy(ArrayList* arl);

String* ArrayList__format(ArrayList* arl);

// Iterator

typedef struct
{
    uint32_t index;
    void* value;
} Iterator(ArrayList);

#define ArrayList__iterator__start(list) { 0, 0 }
#define ArrayList__iterator__test(it, list) (it)->index < (list)->length ? ((it)->value = (list)->values[(it)->index], true) : false
#define ArrayList__iterator__next(it, list) (it)->index ++

// Public Instance Methods - these all borrow the list

bool al_in(ArrayList* arl, uint32_t index); // Checks if a particular index is valid.

void* al_get(ArrayList* arl, uint32_t index); // Gets the element at index i, panics if the index is out of range.

void al_set(ArrayList* arl, uint32_t index, void* value); // Overwrites an element at a specific index. Panics if the index is out of range.
void al_append(ArrayList* arl, void* value); // Appends a new element to the list. Resizes the list if nessecary.

void al_clear(ArrayList* arl); // Removes all elements

#endif


