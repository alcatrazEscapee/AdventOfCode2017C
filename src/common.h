// Collection of all major common language-level feature macros and other related nonsense
//
// Specifications of new Language Features:
//
// new(cls, args...)
// - Creates a new instance of the class 'cls', with constructor arguments 'args'
// - Transfers ownership to the caller
// - This is NOT part of the class struct, and SHOULD be defined for all heap allocated types
//
// del(cls, instance) | del_c(cls_struct, instance)
// - Takes ownership of a instance of the class 'cls' and destroys it.
// - This IS part of the class struct, and MUST be defined.
// 
// copy(cls, instance) | copy_c(cls_struct, instance)
// - Creates a copy of the class name 'cls'. Borrows the existing instance 'instance'
// - Transfers ownership of the new instance to the caller
// - This IS part of the class struct, and MUST be defined, but MAY panic depending on the class.
//
// equals(cls, left, right) | equals_c(cls_struct, instance)
// - Automatically null checks both left and right. NULL == NULL, NULL != anything else
// - Returns true if 'left' and 'right' are semantically equivilant.
// - Borrows both left and right
// - This IS part of the class struct, and MUST be defined.
//
// compare(cls, left, right) | compare_c(cls_struct, left, right)
// - Performs a ordinal comparison on two elements
// - Returns -1 if the 'left' < 'right', returns 1 if 'left' > 'right', and returns 0 if 'left' == 'right'
// - Borrows both left and right
// - Can be indirectly used by the macros compare_lt, compare_gt, compare_le, compare_ge, and the corrosponding class ('_c') variants.
// - This IS part of the class struct, and MUST be defined, but MAY panic depending on the class.
//
// hash(cls, instance) | hash_c(cls_struct, instance)
// - Returns an integer hash code for an object
// - The following relation must hold: equals(cls, a, b) => hash(cls, a) == hash(cls, b)
// - Borrows the instance.
// - This IS part of the class struct, and MUST be defined. It MAY just return 0 (but cannot panic)
//
// class(cls)
// - This obtains the class struct for a given class name.
// - For any of the above methods which have a normal and class ('_c') variant, AND class(cls) is defined, the following must hold:
// - foo(cls, args...) == foo_c(class(cls), args...)
// - This WILL ONLY be defined if cls has a defined class struct.
//
// format(args...) | format_c(cls_struct, instance)
// - Automatically null checks the instance argument
// - This formats a class or other instance to a String
// - Ownership of the String is transfered to the caller
// - By default, this is implemented as returning the class name + the hash code.
// - This IS part of the class struct, and MUST be defined.


#ifndef COMMON_H
#define COMMON_H

// Standard Library Headers
// Dependencies: None

#include <stdio.h> // printf, etc.
#include <stdlib.h> // malloc, free
#include <stdint.h> // int32_t, etc.
#include <stdbool.h> // true, false, bool
#include <string.h> // memcpy
#include <stdarg.h> // Varargs
#include <limits.h> // INT_MIN, INT_MAX
#include <time.h> // time

// Common Structs
// Dependencies: None

struct String__struct;
struct Class__struct;

// Type Definitions
// Dependencies: Struct Declarations

typedef struct String__struct
{
    char* slice; // Backing array
    uint32_t size; // Size of backing array
    /* public readonly */ uint32_t length; // Length of filled string (number of single byte ASCII chars)
} String;

typedef struct Class__struct
{
    void* (*copy_fn)(void*); // Copy
    void (*del_fn)(void*); // Destructor
    bool (*eq_fn)(void*, void*); // Equals
    int32_t (*cmp_fn)(void*, void*); // Compare
    uint32_t (*hash_fn)(void*); // Hash
    struct String__struct* (*fmt_fn)(void*); // Format
    char* name; // Class Name
} Class;

typedef void (*FnDel) (void*);
typedef void* (*FnCopy) (void*);
typedef bool (*FnEquals) (void*, void*);
typedef int32_t (*FnCompare) (void*, void*);
typedef uint32_t (*FnHash) (void*);
typedef struct String__struct* (*FnFormat) (void*);

// Lanuage Level Macros
// Dependencies: None

// Macros for calling class methods through nicer syntax

#define new(cls, args...) cls ## __new(args)
#define del(cls, instance) cls ## __del(instance)
#define copy(cls, instance) cls ## __copy(instance)
#define equals(cls, left, right) ((left) == NULL || (right) == NULL ? (left) == (right) : cls ## __equals(left, right))
#define compare(cls, left, right) cls ## __compare(left, right)
#define hash(cls, instance) cls ## __hash(instance)
#define format(cls, instance) ((instance) == NULL ? NULL_STRING : cls ## __format(instance))

// To obtain a reference to a class object
#define class(cls) & cls ## __class

// Simplifications for compare - only one method is nessecary, but often we just want a simple operator
#define compare_lt(cls, left, right) (compare(cls, left, right) == -1)
#define compare_gt(cls, left, right) (compare(cls, left, right) == 1)
#define compare_le(cls, left, right) (compare(cls, left, right) <= 0)
#define compare_ge(cls, left, right) (compare(cls, left, right) >= 0)

// Macros for obtaining references to class level functions

#define del_f(cls) ((FnDel) (& cls ## __del))
#define copy_f(cls) ((FnCopy) (& cls ## __copy))
#define equals_f(cls) ((FnEquals) (& cls ## __equals))
#define compare_f(cls) ((FnCompare) (& cls ## __compare))
#define hash_f(cls) ((FnHash) (& cls ## __hash))
#define format_f(cls) ((FnFormat) (& cls ## __format))

// Calling methods from a class struct

#define del_c(cls, instance) (cls)->del_fn(instance)
#define copy_c(cls, instance) (cls)->copy_fn(instance)
#define equals_c(cls, left, right) ((cls)->eq_fn((left), (right)))
#define compare_c(cls, left, right) ((cls)->cmp_fn((left), (right)))
#define hash_c(cls, instance) ((cls)->hash_fn(instance))
#define format_c(cls, instance) ((cls)->fmt_fn(instance))

// Compare simplifications - class versions

#define compare_lt_c(cls, left, right) (compare_c(cls, left, right) == -1)
#define compare_gt_c(cls, left, right) (compare_c(cls, left, right) == 1)
#define compare_le_c(cls, left, right) (compare_c(cls, left, right) <= 0)
#define compare_ge_c(cls, left, right) (compare_c(cls, left, right) >= 0)


// Iterators
//
// An iterator over a collection of elements of type T is a struct of at least the following:
// typedef struct {
//     T value;  
// } Iterator(T)
//
// It can be used in place of the condition on a for loop:
// for iter(Class, it, other args...) {
//     it->value
// }
//
// The iterator struct itself is automatically deleted once it goes out of scope of the loop (via clever use of (x ? true : y(), false) semantics)
// Note: if the loop is exited early (through use of return or break), the iterator must be manually cleaned up via del_iter(cls, it)
// In order to define an iterator of type T, the following methods (or macro equivilants) are required:
// Iterator(T) T__iterator__new(args...)
// bool T__iterator__test(Iterator(T)* it, args...)
// void T__iterator__next(Iterator(T)* it, args...)
//
// Additionally, for generic collections, the following variant can be used:
// for type_iter(Class, Type, other args...) {
//     it.parent = an Iterator(Class)
//     it.value = the same as it.parent.value, but as Type.
// }
// This allows iteration over generic data structures without first casting or storing the value (often a void*) down to the desired type.
// For instance, iterating over an ArrayList<String> with iter() would require ((String*) it.value).length, but with type_iter(), it can be referenced as it.value->length

#define Iterator(cls) cls ## __iterator

// Since del(Iterator(Class), ...) will not work due to macro restrictions.
// This sometimes has to be called from user code, e.g. if you break early from an iterator loop
#define del_iter(cls, it) cls ## __iterator__del(it)


#define iter(cls, it, args...) ( \
    Iterator(cls) it = cls ## __iterator__start(args); \
    (cls ## __iterator__test(&it, args)); \
    (cls ## __iterator__next(&it, args)))

#define type_iter(cls, type, it, args...) ( \
    struct { Iterator(cls) parent; type value; } it = { cls ## __iterator__start(args), NULL }; \
    (cls ## __iterator__test(&(it.parent), args)) ? ((it.value = (type) it.parent.value), true) : (false); \
    (cls ## __iterator__next(&(it.parent), args)))


// Class Headers

#define DERIVE_CLASS(cls) \
Class cls ## __class = { \
    .copy_fn = copy_f(cls), \
    .del_fn = del_f(cls), \
    .eq_fn = equals_f(cls), \
    .cmp_fn = compare_f(cls), \
    .hash_fn = hash_f(cls), \
    .fmt_fn = format_f(cls), \
    .name = #cls \
}

// Declares the reference to the class struct, and function declarations for all class methods
// Only thing missing is the constructor, as that's not technically part of the class definition
#define DERIVE_CLASS_HEADER(cls, type) \
extern Class cls ## __class; \
\
void cls ## __del(type instance); \
type cls ## __copy(type instance); \
bool cls ## __equals(type left, type right); \
int32_t cls ## __compare(type left, type right); \
uint32_t cls ## __hash(type instance); \
String* cls ## __format(type instance)


// Class Implementations

// These macros derive common implementation functions
// In most cases, these can be used as-is for basic pointer classes (i.e. Int32 / int32_t* )
// For non-pointer classes (i.e. String), these will need to be implemented individually
// Note 1: there are no macros for automatically defining hash() or new(...)
// Note 2: there are optional macros for defining 'illegal behavior', which are not required to exist for all classes (copy and compare)

#define DERIVE_CLASS_IMPL_ALL(cls, type) \
DERIVE_CLASS_DEL(cls, type) \
DERIVE_CLASS_COPY(cls, type) \
DERIVE_CLASS_EQUALS(cls, type) \
DERIVE_CLASS_COMPARE(cls, type)

#define DERIVE_CLASS_DEL(cls, type) \
void cls ## __del(type v) \
{ \
    free(v); \
}

#define DERIVE_CLASS_COPY(cls, type) \
type cls ## __copy(type v) \
{ \
    return new(cls, *v); \
}

#define DERIVE_CLASS_COPY_PANIC(cls, type) \
type cls ## __copy(type v) \
{ \
    PANIC("Method copy() is not defined for class %s", #cls); \
    return (type) NULL; \
}

#define DERIVE_CLASS_EQUALS(cls, type) \
bool cls ## __equals(type left, type right) \
{ \
    return *left == *right; \
}

#define DERIVE_CLASS_COMPARE(cls, type) \
int32_t cls ## __compare(type left, type right) \
{ \
    return *left == *right ? 0 : (*left < *right ? -1 : 1); \
}

#define DERIVE_CLASS_COMPARE_PANIC(cls, type) \
int32_t cls ## __compare(type left, type right) \
{ \
    PANIC("Method compare() is not defined for class %s", #cls); \
    return 0; \
}

#define DERIVE_CLASS_FORMAT(cls, type) \
String* cls ## __format(type instance) \
{ \
    return str_format("%s#%06u", #cls, hash(cls, instance)); \
}


// Generic Simple Function-Like Macros

#define abs(x) ((x) < 0 ? -(x) : (x))
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))


// Panics
// These, through the use of stringification, provide some level of introspection not gained by a simple exit / error
// Error propogation through return codes is awful (return codes are for return values sheesh)
// These are modeled after Rust's panic!() macro

#define PANIC(args...) do { println("PANIC!"); println(args); exit(1); } while (0)
#define PANIC_IF(condition, args...) do { if (condition) { println("PANIC! Condition %s is false!", #condition); println(args); exit(1); }} while (0)
#define PANIC_IF_NULL(value, args...) do { if (value == NULL) { println("PANIC! %s is NULL!", #value); println(args); exit(1); }} while (0)

// Inclusion of library macros
// Dependencies: Any of the above macros
// Interdependencies: Specified per header

#include "sorting.h"
#include "strings.h"
#include "utils.h"
#include "void.h"

#include "collections/arrayhashmap.h"
#include "collections/arrayhashset.h"
#include "collections/arraylist.h"
#include "collections/intarraylist.h"

#include "math/integer.h"
#include "math/vectors.h"

#endif