// ArrayList data structures
// Defines two types of ArrayLists:
//
// 1. Primitive Typed ArrayLists
// - These are high density data structures for storing any primitive types
// - They are generated through macros
// - They are typed as PrimitiveArrayList(type), and methods are delegated through _Generic macros
//
// 2. Generic ArrayList
// - This is a general purpose ArrayList that has ownership of it's heap allocated elements
// - It is the default behavior for any methods with the 'al' prefix, for non-primitive types
// - It is typed as ArrayList

#include "../lib.h"

#ifndef COLLECTIONS_ARRAY_LIST_H
#define COLLECTIONS_ARRAY_LIST_H

// Array List generic handling for primitive types

#define PrimitiveArrayList(cls) CONCAT(ArrayList_, cls)

#define ARRAY_LIST_GENERIC_PREFIX(list, method) _Generic((list), \
    ArrayList_char : al_char_ ## method, \
    ArrayList_bool : al_bool_ ## method, \
    ArrayList_int32_t : al_int32_t_ ## method, \
    ArrayList_int64_t : al_int64_t_ ## method, \
    ArrayList_uint32_t : al_uint32_t_ ## method, \
    ArrayList_uint64_t : al_uint64_t_ ## method, \
    default: al_pointer_t_ ## method)

// All ArrayList Instance Methods - Delegated through generic prefixing

#define al_get(list, index) ARRAY_LIST_GENERIC_PREFIX(list, get) (list, index)
#define al_set(list, index, value) ARRAY_LIST_GENERIC_PREFIX(list, set) (list, index, value)
#define al_append(list, value) ARRAY_LIST_GENERIC_PREFIX(list, append) (list, value)
#define al_clear(list) ARRAY_LIST_GENERIC_PREFIX(list, clear) (list)

// Primitive Type Array List
// Uses Templating to achieve similar classes with proper line number references

#define type char
#include "primitivearraylist.template.h"

#define type bool
#include "primitivearraylist.template.h"

#define type int32_t
#include "primitivearraylist.template.h"

#define type int64_t
#include "primitivearraylist.template.h"

#define type uint32_t
#include "primitivearraylist.template.h"

#define type uint64_t
#include "primitivearraylist.template.h"

// Iterator Macros for Primitive ArrayLists
// These can't be templated as the type is baked into the function/macro name

#define ArrayList_char__iterator__start(list) { 0, '\0' }
#define ArrayList_bool__iterator__start(list) { 0, false }
#define ArrayList_int32_t__iterator__start(list) { 0, 0 }
#define ArrayList_int64_t__iterator__start(list) { 0, 0 }
#define ArrayList_uint32_t__iterator__start(list) { 0, 0 }
#define ArrayList_uint64_t__iterator__start(list) { 0, 0 }

#define ArrayList_primitive__iterator__test(it, list) (((it)->index < (list)->length) ? (((it)->value = (list)->values[(it)->index]), true) : false)

#define ArrayList_char__iterator__test(it, list) ArrayList_primitive__iterator__test(it, list)
#define ArrayList_bool__iterator__test(it, list) ArrayList_primitive__iterator__test(it, list)
#define ArrayList_int32_t__iterator__test(it, list) ArrayList_primitive__iterator__test(it, list)
#define ArrayList_int64_t__iterator__test(it, list) ArrayList_primitive__iterator__test(it, list)
#define ArrayList_uint32_t__iterator__test(it, list) ArrayList_primitive__iterator__test(it, list)
#define ArrayList_uint64_t__iterator__test(it, list) ArrayList_primitive__iterator__test(it, list)

#define ArrayList_primitive__iterator_next(it, list) ((it)->index ++)

#define ArrayList_char__iterator__next(it, list) ArrayList_primitive__iterator_next(it, list)
#define ArrayList_bool__iterator__next(it, list) ArrayList_primitive__iterator_next(it, list)
#define ArrayList_int32_t__iterator__next(it, list) ArrayList_primitive__iterator_next(it, list)
#define ArrayList_int64_t__iterator__next(it, list) ArrayList_primitive__iterator_next(it, list)
#define ArrayList_uint32_t__iterator__next(it, list) ArrayList_primitive__iterator_next(it, list)
#define ArrayList_uint64_t__iterator__next(it, list) ArrayList_primitive__iterator_next(it, list)

// ArrayList

struct ArrayList__struct
{
    pointer_t* values; // Backing array
    Class value_class; // Values class
    uint32_t size; // Length of backing array
    uint32_t length; // Size of filled elements
};

typedef struct ArrayList__struct * ArrayList;

// Class

declare_class(ArrayList);
declare_constructor(ArrayList, uint32_t initial_size, Class value_class);

// Iterator

typedef struct
{
    uint32_t index;
    pointer_t value;
} Iterator(ArrayList);

#define ArrayList__iterator__start(list) { 0, 0 }
#define ArrayList__iterator__test(it, list) (it)->index < (list)->length ? ((it)->value = (list)->values[(it)->index], true) : false
#define ArrayList__iterator__next(it, list) (it)->index ++

// Public Instance Methods - these all borrow the list

pointer_t al_pointer_t_get(ArrayList list, uint32_t index); // Gets the element at index i, panics if the index is out of range.

void al_pointer_t_set(ArrayList list, uint32_t index, pointer_t value); // Overwrites an element at a specific index. Panics if the index is out of range.
void al_pointer_t_append(ArrayList list, pointer_t value); // Appends a new element to the list. Resizes the list if nessecary.

void al_pointer_t_clear(ArrayList list); // Removes all elements

#endif


