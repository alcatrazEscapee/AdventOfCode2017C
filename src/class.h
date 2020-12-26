// Header for Class objects
// The Class struct is a collection of object methods which are conditionally implemented for various structs
// They are runtime checked for existance using macros
//
// Specifications of new Language Features:
//
// 1. new(cls, args...)
// - Creates a new instance of the class 'cls', with constructor arguments 'args'
// - Transfers ownership to the caller
// - This is NOT part of the class struct, and SHOULD be defined for all heap allocated types
//
// 2. del(cls, instance) | del_c(cls_struct, instance)
// - Takes ownership of a instance of the class 'cls' and destroys it.
// - This IS part of the class struct, and MUST be defined.
// 
// 3. copy(cls, instance) | copy_c(cls_struct, instance)
// - Creates a copy of the class name 'cls'. Borrows the existing instance 'instance'
// - Transfers ownership of the new instance to the caller
// - This IS part of the class struct, and MUST be defined, but MAY panic depending on the class.
//
// 4. equals(cls, left, right) | equals_c(cls_struct, instance)
// - Automatically null checks both left and right. NULL == NULL, NULL != anything else
// - Returns true if 'left' and 'right' are semantically equivilant.
// - Borrows both left and right
// - This IS part of the class struct, and MUST be defined.
//
// 5. compare(cls, left, right) | compare_c(cls_struct, left, right)
// - Performs a ordinal comparison on two elements
// - Returns -1 if the 'left' < 'right', returns 1 if 'left' > 'right', and returns 0 if 'left' == 'right'
// - Borrows both left and right
// - Can be indirectly used by the macros compare_lt, compare_gt, compare_le, compare_ge, and the corrosponding class ('_c') variants.
// - This IS part of the class struct, and MUST be defined, but MAY panic depending on the class.
//
// 6. hash(cls, instance) | hash_c(cls_struct, instance)
// - Returns an integer hash code for an object
// - The following relation must hold: equals(cls, a, b) => hash(cls, a) == hash(cls, b)
// - Borrows the instance.
// - This IS part of the class struct, and MUST be defined.
//
// 7. class(cls)
// - This obtains the class struct for a given class name.
// - For any of the above methods which have a normal and class ('_c') variant, AND class(cls) is defined, the following must hold:
// - foo(cls, args...) == foo_c(class(cls), args...)
// - This WILL ONLY be defined if cls has a defined class struct.
// 
// 8. iter(args...)
// - This iterates through an object. It is meant to be used as iter(...) { .. do stuff with elements .. }
// - Expected arguments are a. the object being iterated, b. any variables that are declared during iteration (such as the index, value, etc.)
// - Due to constraints imposed by macros, this MAY LEAK definitions of variables outside the inner block. Use with care.
// - This is NOT part of the class struct and MAY be defined.
//
// 9. format(args...) | format_c(cls_struct, instance)
// - Automatically null checks the instance argument
// - This formats a class or other instance to a String
// - Ownership of the String is transfered to the caller
// - By default, this is implemented as returning the class name + the hash code.
// - This IS part of the class struct, and MUST be defined.

#ifndef CLASS_H
#define CLASS_H

// Class struct
// A single stack allocated instance should exist for every used class at runtime
// Declare using the DERIVE_CLASS and DERIVE_CLASS_HEADER macros

struct Class__struct;

typedef struct Class__struct {
    void* (*copy_fn)(void*); // Copy
    void (*del_fn)(void*); // Destructor
    bool (*eq_fn)(void*, void*); // Equals
    int32_t (*cmp_fn)(void*, void*); // Compare
    uint32_t (*hash_fn)(void*); // Hash
    struct String__struct* (*fmt_fn)(void*); // Format
    char* name; // Class Name
} Class;

// Macros for calling class level functions through nicer syntax

#define new(cls, args...) cls ## __new(args)
#define del(cls, instance) cls ## __del(instance)
#define copy(cls, instance) cls ## __copy(instance)
#define equals(cls, left, right) ((left) == NULL || (right) == NULL ? (left) == (right) : cls ## __equals(left, right))
#define compare(cls, left, right) cls ## __compare(left, right)
#define hash(cls, instance) cls ## __hash(instance)
#define format(cls, instance) ((instance) == NULL ? NEW_NULL_STRING : cls ## __format(instance))

// Simplifications for compare - only one method is nessecary, but often we just want a simple operator
#define compare_lt(cls, left, right) (compare(cls, left, right) == -1)
#define compare_gt(cls, left, right) (compare(cls, left, right) == 1)
#define compare_le(cls, left, right) (compare(cls, left, right) <= 0)
#define compare_ge(cls, left, right) (compare(cls, left, right) >= 0)

// This one is technically not a class method but it's a standard macro -> language feature
#define iter(cls, args...) cls ## __iter(args)

// To obtain a reference to a class object
#define class(cls) & cls ## __class

// Since constructors are not part of the class object but still need to be defined
// This makes constructor methods easilly recognizable - i.e. T constructor(T)(...) { ... }
#define constructor(cls) cls ## __new

// Type definitions for class functions
typedef void (*FnDel) (void*);
typedef void* (*FnCopy) (void*);
typedef bool (*FnEquals) (void*, void*);
typedef int32_t (*FnCompare) (void*, void*);
typedef uint32_t (*FnHash) (void*);
typedef struct String__struct* (*FnFormat) (void*);

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

// Macros for class declaration

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

// Expansions for basic class behavior

// Class header
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

// Implementations

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

#endif
