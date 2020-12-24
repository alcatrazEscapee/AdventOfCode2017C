// Header for Class objects
// The Class struct is a collection of object methods which are conditionally implemented for various structs
// They are runtime checked for existance using macros

#ifndef CLASS_H
#define CLASS_H

#include "std.h"
#include "panic.h"

// Class struct
// A single stack allocated instance should exist for every used class at runtime

struct Class__struct;

typedef struct Class__struct {
    void* (*copy_fn)(void*); // Copy
    void (*del_fn)(void*); // Destructor
    bool (*eq_fn)(void*, void*); // Equals
    int32_t (*cmp_fn)(void*, void*); // Compare
    uint32_t (*hash_fn)(void*); // Hash
    char* name; // Class Name
} Class;

// Macros for calling class level functions through nicer syntax

#define new(cls, args...) cls ## __new(args)
#define del(cls, instance) cls ## __del(instance)
#define copy(cls, instance) cls ## __copy(instance)
#define equals(cls, left, right) cls ## __equals(left, right)
#define compare(cls, left, right) cls ## __compare(left, right)
#define hash(cls, instance) cls ## __hash(instance)

// This one is technically not a class method but it's a standard macro -> language feature
#define iter(cls, args...) cls ## __iter(args)

// To obtain a reference to a class object
#define class(cls) & cls ## __class

// Since constructors are not part of the class object but still need to be defined
#define constructor(cls) cls ## __new

// Type definitions for class functions
typedef void (*FnDel) (void*);
typedef void* (*FnCopy) (void*);
typedef bool (*FnEquals) (void*, void*);
typedef int32_t (*FnCompare) (void*, void*);
typedef uint32_t (*FnHash) (void*);

// Macros for obtaining references to class level functions

#define del_f(cls) ((FnDel) (& cls ## __del))
#define copy_f(cls) ((FnCopy) (& cls ## __copy))
#define equals_f(cls) ((FnEquals) (& cls ## __equals))
#define compare_f(cls) ((FnCompare) (& cls ## __compare))
#define hash_f(cls) ((FnHash) (& cls ## __hash))

// Calling methods from a class struct

#define del_c(cls, instance) (cls)->del_fn(instance)
#define copy_c(cls, instance) (cls)->copy_fn(instance)
#define equals_c(cls, left, right) ((cls)->eq_fn((left), (right)))
#define compare_c(cls, left, right) ((cls)->cmp_fn((left), (right)))
#define hash_c(cls, instance) ((cls)->hash_fn(instance))

// Macros for class declaration

#define CLASS(cls, copy, del, eq, cmp, hash) Class cls ## __class = { .copy_fn = copy, .del_fn = del, .eq_fn = eq, .cmp_fn = cmp, .hash_fn = hash, .name = #cls }
#define DERIVE_CLASS(cls) CLASS(cls, copy_f(cls), del_f(cls), equals_f(cls), compare_f(cls), hash_f(cls))

// Expansions for basic class behavior

// Class header
// Declares the reference to the class struct, and function declarations for all class methods
// Only thing missing is the constructor, as that's not technically part of the class definition
#define DERIVE_CLASS_HEADER(cls, type) \
extern Class cls ## __class; \
\
void cls ## __del(type instance); \
type cls ## __copy(type instance); \
\
bool cls ## __equals(type left, type right); \
int32_t cls ## __compare(type left, type right); \
uint32_t cls ## __hash(type instance);

// Implementations

// This simply derives all common implementations for basic pointer types

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

#define DERIVE_PANIC(cls, method) PANIC("Method %s is not defined for class %s", #cls, #method)

#endif
