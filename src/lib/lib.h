#ifndef COMMON_H
#define COMMON_H

// Standard Library Headers

#include <stdio.h> // printf, etc.
#include <stdlib.h> // malloc, free
#include <stdint.h> // int32_t, etc.
#include <stdbool.h> // true, false, bool
#include <string.h> // memcpy
#include <stdarg.h> // Varargs
#include <limits.h> // INT_MIN, INT_MAX
#include <time.h> // time
#include <ctype.h> // isdigit
#include <setjmp.h> // setjmp, longjmp, jmp_buf

// In stdbool.h, bool is macro-defined to equal _Bool
// This causes us subtle problems with class methods defined for primitive types. For example:
// equals(bool, a, b)
// This SHOULD expand to the macro __equals_p(bool, a, b), which then is further expanded to just ((a) == (b))
// However, with this definition, it actually expands to _Bool__equals(_Bool, a, b), treating it as a non-primitive type
// This is undesired, and rather than special casing all the boolean logic to allow bool and _Bool, we instead just typedef it like it should've been in the first place
#undef bool
typedef _Bool bool;

// Primitive Types:
// char, bool, int32_t, int64_t, uint32_t, uint64_t

// Primitive Reference Types:
// These are the ONLY omissions from handling with del, equals, etc.
// Reason: they should NEVER be called as such. del(pointer_t, t) should be more accurately specified, and del(slice_t, t) should use String instead
// slice_t, pointer_t

typedef char* slice_t;
typedef void* pointer_t;

// Primitive Type Handling

// A macro that is defined for all primitive and class types
// It returns PRIMITIVE_TYPE for primitive types, and CLASS_TYPE for classes
#define typeof(cls) IIF( IS_PRIMITIVE_TYPE(cls) ) (PRIMITIVE_TYPE, CLASS_TYPE)

#define PRIMITIVE_TYPE p
#define CLASS_TYPE c

#define IS_PRIMITIVE_TYPE(cls) CHECK(PRIMITIVE_TYPE_PROBE(cls))

#define PRIMITIVE_TYPE_PROBE(cls)          PRIMITIVE_TYPE_PROBE_PROXY( CONCAT(cls, __PRIMITIVE_TYPE) ) // concatenate prefix with user name
#define PRIMITIVE_TYPE_PROBE_PROXY(...)    PRIMITIVE_TYPE_PROBE_MERGE(__VA_ARGS__)               // expand arguments
#define PRIMITIVE_TYPE_PROBE_MERGE(x)      PRIMITIVE_TYPE_PROBE_COMBINE_ x                       // merge
#define PRIMITIVE_TYPE_PROBE_COMBINE_(...) PROBE(~)                                              // if merge successful, expand to probe

#define char__PRIMITIVE_TYPE ()
#define bool__PRIMITIVE_TYPE ()
#define int32_t__PRIMITIVE_TYPE ()
#define int64_t__PRIMITIVE_TYPE ()
#define uint32_t__PRIMITIVE_TYPE ()
#define uint64_t__PRIMITIVE_TYPE ()

// Macro Wizardry
// Allows us to do checks against specific macro values (i.e. for primitives)

// Contatenations that work w.r.t. brackets
#define CONCAT(a, ...) CONCAT_(a, __VA_ARGS__)
#define CONCAT_(a, ...) a ## __VA_ARGS__

// Concats all individual arguments rather than taking the second to be varadic
// CONCAT(a, b, c) will produce "ab, c"
// CONCAT3(a, b, c) will produce "abc"
#define CONCAT3(x0, x1, x2) CONCAT(x0, CONCAT(x1, x2))
#define CONCAT4(x0, x1, x2, x3) CONCAT3(x0, x1, CONCAT(x2, x3))
#define CONCAT5(x0, x1, x2, x3, x4) CONCAT4(x0, x1, x2, CONCAT(x3, x4))
#define CONCAT6(x0, x1, x2, x3, x4, x5) CONCAT5(x0, x1, x2, x3, CONCAT(x4, x5))

// Stringification of macro values
#define JOIN(args...) args
#define LITERAL(args...) LITERAL_(args)
#define LITERAL_(args...) #args
 
// Conditional Switching
#define IIF(c) CONCAT(IIF_, c)
#define IIF_0(t, ...) __VA_ARGS__
#define IIF_1(t, ...) t

// Conditional Statements
#define CHECK(...) CHECK_N(__VA_ARGS__, 0)
#define CHECK_N(x, n, ...) n

#define PROBE(x) x, 1 


// Primitive Type Classes:
// Character, Boolean, Int32, Int64, UInt32, UInt64

typedef char*     Character;
typedef bool*     Boolean;
typedef int32_t*  Int32;
typedef int64_t*  Int64;
typedef uint32_t* UInt32;
typedef uint64_t* UInt64;

// Structs and Type Definitions for Library Classes

struct String__struct
{
    slice_t slice; // Backing array
    uint32_t size; // Size of backing array
    uint32_t length; // Length of filled string (number of single byte ASCII chars)
};

typedef struct String__struct* String;

struct Class__struct
{
    pointer_t (*copy_fn)(pointer_t); // Copy
    void      (*del_fn) (pointer_t); // Destructor
    bool      (*eq_fn)  (pointer_t, pointer_t); // Equals
    int32_t   (*cmp_fn) (pointer_t, pointer_t); // Compare
    uint32_t  (*hash_fn)(pointer_t); // Hash
    String    (*fmt_fn) (pointer_t); // Format
    slice_t   name; // Class Name
};

typedef struct Class__struct* Class;

// Common Class Methods

typedef void      (*FnDel)     (pointer_t);
typedef pointer_t (*FnCopy)    (pointer_t);
typedef bool      (*FnEquals)  (pointer_t, pointer_t);
typedef int32_t   (*FnCompare) (pointer_t, pointer_t);
typedef uint32_t  (*FnHash)    (pointer_t);
typedef String    (*FnFormat)  (pointer_t);

// Class Methods - Definitions

// Calling Macros
// These delegate to either the class, or primitive versions by using the typeof() macro
#define new(cls, args...) CONCAT(cls, __new) (args)

#define del(cls, instance) CONCAT(__del_, typeof(cls)) (cls, instance)
#define copy(cls, instance) CONCAT(__copy_, typeof(cls)) (cls, instance)
#define equals(cls, left, right) CONCAT(__equals_, typeof(cls)) (cls, left, right)
#define compare(cls, left, right) CONCAT(__compare_, typeof(cls)) (cls, left, right)
#define hash(cls, instance) CONCAT(__hash_, typeof(cls)) (cls, instance)
#define format(cls, instance) CONCAT(__format_, typeof(cls)) (cls, instance)

// Default value of a class. For primitive types this is the relavant NULL / 0 / '\0', for all classes this is NULL
#define default_value(cls) CONCAT(__default_value_, typeof(cls)) (cls)

// Simplifications for compare - only one method is nessecary, but often we just want a simple operator
#define compare_lt(cls, left, right) (compare(cls, left, right) == -1)
#define compare_gt(cls, left, right) (compare(cls, left, right) == 1)
#define compare_le(cls, left, right) (compare(cls, left, right) <= 0)
#define compare_ge(cls, left, right) (compare(cls, left, right) >= 0)

// Primitive Types
#define __del_p(cls, instance) ((void)0)
#define __copy_p(cls, instance) (instance)
#define __equals_p(cls, left, right) ((left) == (right))
#define __compare_p(cls, left, right) (((left) == (right)) ? 0 : (((left) < (right)) ? -1 : 1))
#define __hash_p(cls, instance) CONCAT(cls, __hash)(instance)
#define __format_p(cls, instance) CONCAT(cls, __format)(instance)

#define __default_value_p(cls) CONCAT(cls, __default_value)

#define char__hash(instance)     ((uint32_t) instance)
#define bool__hash(instance)     ((uint32_t) instance)
#define int32_t__hash(instance)  ((uint32_t) instance)
#define int64_t__hash(instance)  ((uint32_t)((instance) | ((instance) >> 32)))
#define uint32_t__hash(instance) ((uint32_t) instance)
#define uint64_t__hash(instance) ((uint32_t)((instance) | ((instance) >> 32)))

#define char__format(instance)     str_format("%c", instance)
#define bool__format(instance)     new(String, ((instance) ? "true" : "false"))
#define int32_t__format(instance)  str_format("%d", instance)
#define int64_t__format(instance)  str_format("%ld", instance)
#define uint32_t__format(instance) str_format("%u", instance)
#define uint64_t__format(instance) str_format("%lu", instance)

#define char__default_value '\0'
#define bool__default_value false
#define int32_t__default_value ((int32_t)0)
#define int64_t__default_value ((int64_t)0)
#define uint32_t__default_value ((uint32_t)0)
#define uint64_t__default_value ((uint64_t)0)


// Class Types
#define __del_c(cls, instance) __del(instance, del_f(cls))
#define __copy_c(cls, instance) __copy(instance, copy_f(cls))
#define __equals_c(cls, left, right) __equals(left, right, equals_f(cls))
#define __compare_c(cls, left, right) __compare(left, right, compare_f(cls))
#define __hash_c(cls, instance) __hash(instance, hash_f(cls))
#define __format_c(cls, instance) __format(instance, format_f(cls))

#define __default_value_c(cls) NULL

// References to Class Methods

#define del_f(cls) ((FnDel) (& CONCAT(cls, __del)))
#define copy_f(cls) ((FnCopy) (& CONCAT(cls, __copy)))
#define equals_f(cls) ((FnEquals) (& CONCAT(cls, __equals)))
#define compare_f(cls) ((FnCompare) (& CONCAT(cls, __compare)))
#define hash_f(cls) ((FnHash) (& CONCAT(cls, __hash)))
#define format_f(cls) ((FnFormat) (& CONCAT(cls, __format)))

// Passthrough Functions for Class Methods
// These do some basic null checks and other standard required behavior, and require that the macro does not duplicate an expression
// For example, consider the following macro:
// #define copy(cls, instance) (instance == NULL) ? NULL : cls ## __copy(instance)
// Then the usage:
// copy(T, t = new(T))
// It would produce the following:
// (t = new(T) == NULL) ? NULL : T__copy(t = new(T))
// Which is both invalid, unexpected, and a horrible memory leak.
// So, in order to avoid this, the macros simply wrap function calls and pass named functions as parameters

void __del(pointer_t instance, FnDel f);
pointer_t __copy(pointer_t instance, FnCopy f);
bool __equals(pointer_t left, pointer_t right, FnEquals f);
int32_t __compare(pointer_t left, pointer_t right, FnCompare f);
uint32_t __hash(pointer_t instance, FnHash f);
String __format(pointer_t instance, FnFormat f);

// Class Methods, variants using a Class<T> rather than a Class

// Converts a Class -> Class<T>
#define class(cls) (& CONCAT(cls, __class))

// Variants which take a Class<T> instead of a Class
#define del_c(cls, instance) (cls)->del_fn(instance)
#define copy_c(cls, instance) (cls)->copy_fn(instance)
#define equals_c(cls, left, right) ((cls)->eq_fn((left), (right)))
#define compare_c(cls, left, right) ((cls)->cmp_fn((left), (right)))
#define hash_c(cls, instance) ((cls)->hash_fn(instance))
#define format_c(cls, instance) ((cls)->fmt_fn(instance))

#define compare_lt_c(cls, left, right) (compare_c(cls, left, right) == -1)
#define compare_gt_c(cls, left, right) (compare_c(cls, left, right) == 1)
#define compare_le_c(cls, left, right) (compare_c(cls, left, right) <= 0)
#define compare_ge_c(cls, left, right) (compare_c(cls, left, right) >= 0)


// Macros for Defining Classes
// Three considerations:
// 1. Primitive Type Classes (for each primitive type defined in this library)
// 2. Tuple Classes (simple N-tuple classes with arbitrary primitive/class values)
// 3. Standard Classes (classes with arbitrary behavior)

// Inclusion in header files:

#define declare_class(cls) \
extern struct Class__struct CONCAT(cls, __class); \
\
void CONCAT(cls, __del)(cls instance); \
cls CONCAT(cls, __copy)(cls instance); \
bool CONCAT(cls, __equals)(cls left, cls right); \
int32_t CONCAT(cls, __compare)(cls left, cls right); \
uint32_t CONCAT(cls, __hash)(cls instance); \
String CONCAT(cls, __format)(cls instance)

#define declare_constructor(cls, args...) cls CONCAT(cls, __new) (args)

// Inclusion in source files:

#define impl_class(cls) \
struct Class__struct CONCAT(cls, __class) = { \
    .copy_fn = copy_f(cls), \
    .del_fn = del_f(cls), \
    .eq_fn = equals_f(cls), \
    .cmp_fn = compare_f(cls), \
    .hash_fn = hash_f(cls), \
    .fmt_fn = format_f(cls), \
    .name = LITERAL(cls) \
}

// Primitive Classes
// These should only be used for standard library primitive classes

#define declare_primitive_class(cls, type) \
declare_class(cls); \
declare_constructor(cls, type value)

#define impl_primitive_class(cls, type) \
impl_class(cls); \
cls cls ## __new(type value) { cls box = safe_malloc(cls, sizeof(type)); *box = value; return box; } \
void cls ## __del(cls instance) { free(instance); } \
cls cls ## __copy(cls instance) { return new(cls, *(instance)); } \
bool cls ## __equals(cls left, cls right) { return equals(type, *left, *right); } \
int32_t cls ## __compare(cls left, cls right) { return compare(type, *left, *right); } \
uint32_t cls ## __hash(cls instance) { return hash(type, *instance); } \
String cls ## __format(cls instance) { return format(type, *instance); }

// Declarations of Primitive Box Classes

declare_primitive_class(Character, char);
declare_primitive_class(Boolean, bool);
declare_primitive_class(Int32, int32_t);
declare_primitive_class(Int64, int64_t);
declare_primitive_class(UInt32, uint32_t);
declare_primitive_class(UInt64, uint64_t);


// Declarations of Class Meta-Class, and Void Class
// No constructors, and very simple implementations
declare_class(Class);

typedef void* Void;
declare_class(Void);

// Tuples!
// Tuples are simple auto-classes that can contain up to four primitive or class types.

// Both are required to create and use a tuple - the declaration can be put in a header file, the impl should not.
#define declare_tuple(cls, args...) TUPLE_UNROLL(TUPLE_HEADER, cls, args)
#define impl_tuple(cls, args...) TUPLE_UNROLL(TUPLE_IMPL, cls, args)

// Unrolls the arguments to the tuple and selects the right TUPLE_# executor based on how many arguments were provided
#define TUPLE_UNROLL(exec, cls, args...) TUPLE_UNROLL_SELECT(args, TUPLE_4, ~, TUPLE_3, ~, TUPLE_2, ~, TUPLE_1) (exec, cls, args)
#define TUPLE_UNROLL_SELECT(t1, v1, t2, v2, t3, v3, t4, v4, x, ...) x

// Expands the given arguments to the tuple function (either TUPLE_HEADER or TUPLE_IMPL)
// Arguments are ALL type value pairs, followed by the total number of arguments (nargs), and then a boolean (1 or 0) for each optional argument
// The 'x's are placeholders - they must be valid tokens, even though they are discarded
#define TUPLE_1(exec, cls, type1, value1) exec(cls, type1, value1, x, x, x, x, x, x, 1, 0, 0, 0)
#define TUPLE_2(exec, cls, type1, value1, type2, value2) exec(cls, type1, value1, type2, value2, x, x, x, x, 2, 1, 0, 0)
#define TUPLE_3(exec, cls, type1, value1, type2, value2, type3, value3) exec(cls, type1, value1, type2, value2, type3, value3, x, x, 3, 1, 1, 0)
#define TUPLE_4(exec, cls, type1, value1, type2, value2, type3, value3, type4, value4) exec(cls, type1, value1, type2, value2, type3, value3, type4, value4, 4, 1, 1, 1)

// If arg == 1, output value, else nothing. Used for no-op statements in tuple boilerplate code.
#define IIF_EMPTY(arg, value) IIF ( arg ) (value,)

// Generates a list of nargs elements
// Usage: IIF_LIST(nargs) (K, arg2, arg3... argN) will return arg1, arg2... argK for K <= N
#define IIF_LIST(nargs) IIF_LIST_ ## nargs
#define IIF_LIST_1(arg1, ...) arg1
#define IIF_LIST_2(arg1, arg2, ...) arg1, arg2
#define IIF_LIST_3(arg1, arg2, arg3, ...) arg1, arg2, arg3
#define IIF_LIST_4(arg1, arg2, arg3, arg4, ...) arg1, arg2, arg3, arg4

// Tuple header
// Defines a struct containing each type-value pair, and the required header to declare it as a class
#define TUPLE_HEADER(cls, type1, value1, type2, value2, type3, value3, type4, value4, nargs, arg2, arg3, arg4) \
struct cls ## __struct \
{ \
    type1 value1; \
    IIF_EMPTY(arg2, type2 value2;) \
    IIF_EMPTY(arg3, type3 value3;) \
    IIF_EMPTY(arg4, type4 value4;) \
}; \
\
typedef cls ## __struct * cls; \
\
declare_class(cls); \
declare_constructor(cls, IIF_LIST(nargs)(type1 value1, type2 value2, type3 value3, type4 value4)) \

// Tuple implementation
// Implements the class struct, and defines standard tuple behavior for all class methods.
#define TUPLE_IMPL(cls, type1, value1, type2, value2, type3, value3, type4, value4, nargs, arg2, arg3, arg4) \
impl_class(cls); \
\
cls cls ## __new( IIF_LIST(nargs)(type1 value1, type2 value2, type3 value3, type4 value4) ) \
{ \
    cls tuple = class_malloc(cls); \
    tuple->value1 = value1; \
    IIF_EMPTY(arg2, tuple->value2 = value2;) \
    IIF_EMPTY(arg3, tuple->value3 = value3;) \
    IIF_EMPTY(arg4, tuple->value4 = value4;) \
    return tuple; \
} \
void cls ## __del(cls tuple) \
{ \
    del(type1, tuple->value1); \
    IIF_EMPTY(arg2, del(type2, tuple->value2);) \
    IIF_EMPTY(arg3, del(type3, tuple->value3);) \
    IIF_EMPTY(arg4, del(type4, tuple->value4);) \
    free(tuple); \
} \
void cls ## __copy(cls tuple) \
{ \
    return new(cls,  IIF_LIST(nargs)(copy(type1, tuple->value1), copy(type2, tuple->value2), copy(type3, tuple->value3), copy(type4, tuple->value4)) ); \
} \
bool cls ## __equals(cls left, cls right) \
{ \
    return equals(type1, left->value1, right->value1) \
        IIF_EMPTY(arg2, && equals(type2, left->value2, right->value2)) \
        IIF_EMPTY(arg3, && equals(type3, left->value3, right->value3)) \
        IIF_EMPTY(arg4, && equals(type4, left->value4, right->value4)) \
    ; \
} \
int32_t cls ## __compare(cls left, cls right) \
{ \
    int32_t result = compare(type1, left->value1, right->value1);\
    if (result != 0) return result; \
    IIF_EMPTY(arg2, result = compare(type2, left->value2, right->value2); if (result != 0) return result;) \
    IIF_EMPTY(arg3, result = compare(type3, left->value3, right->value3); if (result != 0) return result;) \
    IIF_EMPTY(arg4, result = compare(type4, left->value4, right->value4); if (result != 0) return result;) \
    return result; \
} \
uint32_t cls ## __hash(cls tuple) \
{ \
    uint32_t h = hash(type1, tuple->value1); \
    IIF_EMPTY(arg2, h ^= hash(type2, tuple->value2);) \
    IIF_EMPTY(arg3, h ^= hash(type3, tuple->value3);) \
    IIF_EMPTY(arg4, h ^= hash(type4, tuple->value4);) \
    return h; \
} \
String cls ## __format(cls tuple) \
{ \
    String string = new(String, #cls "("); \
    str_append_string(string, format(type1, tuple->value1)); \\
    str_append_slice(string, ", "); \
    IIF_EMPTY(arg2, str_append_string(string, format(type2, tuple->value2)); str_append_slice(string, ", ");) \
    IIF_EMPTY(arg3, str_append_string(string, format(type3, tuple->value3)); str_append_slice(string, ", ");) \
    IIF_EMPTY(arg4, str_append_string(string, format(type4, tuple->value4)); str_append_slice(string, ", ");) \
    str_pop(string, 2); \
    str_append_char(string, ')'); \
    return string; \
}


// Panics
// These are modeled after Rust's panic!() macro
// Using stringification, they can automatically provide a bit more error context
//
// Additionally, they make use of the C setjmp / longjmp statements for a single, global error handling context
// This is used by unit testing, in order to run sandboxed test code that may panic, and recover from it rather than exiting

#define panic(format_string, args...) do { \
    PANIC_INTERNAL("  at: panic(" LITERAL(JOIN(format_string, ## args)) ")", format_string, ## args); \
} while (0)

#define panic_if(condition, format_string, args...) do { \
    if (condition) { \
        PANIC_INTERNAL("  at: panic_if(" LITERAL(JOIN(condition, format_string, ## args)) ")", format_string, ## args ); \
    } \
} while (0)

#define panic_if_null(pointer, format_string, args...) do { \
    if (pointer == NULL) { \
        PANIC_INTERNAL("  at: panic_if_null(" LITERAL(JOIN(pointer, format_string, ## args)) ")", format_string, ## args ); \
    } \
} while (0)

// The macro that actually executes a panic
// First, create and store the exception text
// If we're in an exception handling context, use longjmp to return back to the catch {} block.
// Otherwise, forcefully exit after printing the exception text
#define PANIC_INTERNAL(literal_string, format_string, args...) \
do { \
    String __exception_error = str_format(format_string, ## args); \
    slice_t __exception_try_slice = __exception_active ? __exception_try_value->slice : ""; \
    __exception_value = str_format("%s\n%s (" __FILE__ ":%d)\n  at: %s(...) (" __FILE__ ":?)\n%s",  __exception_error->slice, literal_string, __LINE__, __func__, __exception_try_slice); \
    del(String, __exception_error); \
    if (__exception_active) { \
        longjmp(__exception_jmp_buf, 1); \
    } else { \
        printf("Program Paniced!\n%s", __exception_value->slice); \
        del(String, move(__exception_value)); \
        del(String, move(__exception_try_value)); \
        exit(1); \
    } \
} while (0)

// Try Handling
// Usage:
// try
// {
//     // code that WILL NOT invoke try, but MAY invoke a panic() variant
//     // IF return; is invoked, all possible code that may panic MUST be prefixed with a finally; statement:
//     int32_t x = complex_result();
//     finally;
//     return x;
// }
// catch
// {
//     // will be executed ONLY if the try block paniced
// }
// finally;

extern bool    __exception_active;
extern jmp_buf __exception_jmp_buf;
extern String  __exception_try_value;
extern String  __exception_value;

#define exception copy(String, __exception_value)

#define try do {\
    if (__exception_active) \
    { \
        panic("Nested try/catch not supported!"); \
    } \
    __exception_active = true; \
    __exception_try_value = str_format("  at: try (" __FILE__ ":%d)\n", __LINE__); \
} while (0); \
if (!setjmp(__exception_jmp_buf))

#define catch else
#define finally do { \
    __exception_active = false; \
    del(String, move(__exception_try_value)); \
    del(String, move(__exception_value)); \
} while (0)


// Standard Memory Allocations
#define class_malloc(cls) __malloc("Class<" LITERAL(cls) ">", sizeof(struct CONCAT(cls, __struct)))
#define safe_malloc(cls, size) __malloc(LITERAL(cls), size)

// Calls malloc() guarded with an panic
// Return value is gaurenteed to be non null
pointer_t __malloc(slice_t name, uint32_t size);

// Rust style loop statements
#define loop while (true)

// Collection methods for iterators
// Inputs are an iter() statement, a new collection, and a statement which accumulates elements of the iterator into the collection
// Usage:
// ArrayList* list = collect(iter(...), new(...), ...)
#define collect(iter_statement, collection, collector_statement) collection; { for iter_statement { collector_statement; }}


// Move / Ownership Semantics
//
// Most functions have specific contracts about how they manage ownership of pointers
// There are a few notions, simplified from C++ / C practices, and Rust:
//
// 1. Borrowing:
// - A pointer is passed by value (T* t) to a function.
// - The function MUST NOT call del(T, t) during it's execution, and at the return of the function, the pointer must still be valid
//
// 2. Moving:
// - A pointer is passed by reference (T** t) to a function.
// - The pointer may be destroyed at the end of this function. (e.g. the function called either del(T, *t) or v = move(T, *t))
// - This is heavilly context dependent. It may apply, e.g. to iterator methods.
// - In the body of an iterator, the main code MAY take ownership of the internal value by calling move(T, &t). This value will then by NULL'd (and the required call to del(T, t) will noop)
//
// 3. Destroying
// - A pointer is passed by value (T* t) to a function
// - The function takes ownership of the pointer, and it IS NOT valid to use it after passing it, or call del(T, t).
// - Examples include del(T, t), which takes ownership of 't'
//
// You can only call move() when given a MUTABLE BORROW (which must be unique)
// Then, the original value is nullified, there are no other references, and you now have ownership of the reference
// Thus, a BORROW cannot be upgraded into ownership, but a MUTABLE BORROW can be.
// For example, an iterator through an ArrayList<String> gives a IMMUTABLE BORROW. There are two references (the list, owner, and the iterator, borrowing) to the value
// Another example, the StringLines iterator owns each string instance, and the loop code is given a MUTABLE BORROW. It can take ownership through calling move()

#define move(ptr) (__move((void**)(& (ptr))))
void* __move(void** ref_ptr);


// Iterators
//
// An iterator over a collection of elements of type T is a struct of at least the following:
// typedef struct {
//     T value;  
// } Iterator<T>
//
// It can be used in place of the condition on a for loop:
// for iter(Class, it, other args...) {
//     it.value
// }
//
// The iterator struct itself is stack allocated and does not need to be cleaned up.
// In order to define an iterator of type T, the following methods (or macro equivilants) are required:
// Iterator<T> T__iterator__start(args...)
// bool T__iterator__test(Iterator<T>* it, args...)
// void T__iterator__next(Iterator<T>* it, args...)
//
// Additionally, for generic collections, the following variant can be used:
// for type_iter(Class, Type, other args...) {
//     it.parent = an Iterator<Class>
//     it.value = the same as it.parent.value, but as Type
// }
// This allows iteration over generic data structures without first casting or storing the value (often a void*) down to the desired type.
// For instance, iterating over an ArrayList<String> with iter() would require ((String*) it.value)->length, but with type_iter(), it can be referenced as it.value->length

#define Iterator(cls) CONCAT(cls, __iterator)

#define iter(cls, it, args...) ( \
    Iterator(cls) it = CONCAT(cls, __iterator__start)(args); \
    (CONCAT(cls, __iterator__test)(&it, args)); \
    (CONCAT(cls, __iterator__next)(&it, args)))

#define type_iter(cls, type, it, args...) ( \
    struct { Iterator(cls) parent; type value; } it = { CONCAT(cls, __iterator__start)(args), NULL }; \
    (CONCAT(cls, __iterator__test)(&(it.parent), args)) ? ((it.value = (type) it.parent.value), true) : (false); \
    (CONCAT(cls, __iterator__next)(&(it.parent), args)))

// Generic Simple Function-Like Macros

#define abs(x) ((x) < 0 ? -(x) : (x))
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

// Inclusion of library macros
// Dependencies: Any of the above macros
// Interdependencies: Specified per header

#include "utils.h"
#include "strings.h"

#include "collections/arraylist.h"
#include "collections/map.h"
#include "collections/set.h"

#endif