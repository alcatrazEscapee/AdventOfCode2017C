#ifndef COMMON_H
#define COMMON_H

// Generic Macro Wizardry

// Contatenations that work w.r.t. other macro expansion
// #define A() X
// A() ## Y will produce A()Y
// CONCAT(A(), Y) will produce XY
#define CONCAT(a, ...) CONCAT_(a, __VA_ARGS__)
#define CONCAT_(a, ...) a ## __VA_ARGS__

// Concats all individual arguments rather than taking the second to be varadic
// CONCAT(a, b, c) will produce "ab, c"
// CONCAT3(a, b, c) will produce "abc"
#define CONCAT3(a, b, ...) CONCAT(CONCAT(a, b), __VA_ARGS__)
#define CONCAT4(a, b, ...) CONCAT3(CONCAT(a, b), __VA_ARGS__)
#define CONCAT5(a, b, ...) CONCAT4(CONCAT(a, b), __VA_ARGS__)
#define CONCAT6(a, b, ...) CONCAT5(CONCAT(a, b), __VA_ARGS__)
#define CONCAT7(a, b, ...) CONCAT6(CONCAT(a, b), __VA_ARGS__)
#define CONCAT8(a, b, ...) CONCAT7(CONCAT(a, b), __VA_ARGS__)

// Stringification of macro values that works w.r.t other macro expansion
// These also work outside of existing macros, i.e. in template files
#define LITERAL(args...) LITERAL_(args)
#define LITERAL_(args...) #args

// Joins multiple discrete arguments into a single varadic args call
#define JOIN(args...) args

// Conditional Switching. Accepts IIF ( condition ) (value_if_false, value_if_true ), where condition resolves to 0 or 1
#define IIF(c) CONCAT(IIF_, c)
#define IIF_0(t, ...) __VA_ARGS__
#define IIF_1(t, ...) t

// Conditional Statements
#define CHECK(...) CHECK_N(__VA_ARGS__, 0)
#define CHECK_N(x, n, ...) n

#define PROBE(x) x, 1

// Reflective Macro Invocation
// #define ARGS 1, 2, 3
// #define SUM(a, b, c) a + b + c
// SUM(ARGS) will produce an error due to too few parameters
// REFLECT(SUM, ARGS) will produce 1 + 2 + 3
#define REFLECT(m, ...) REFLECT_EXPAND(m REFLECT_WRAP(__VA_ARGS__))
#define REFLECT_EXPAND(x) x
#define REFLECT_WRAP(...) (__VA_ARGS__)

// Extracting arguments from a list
// Arguments are 1-indexed
#define ARG_1(x, ...)  x
#define ARG_2(x, ...)  ARG_1(__VA_ARGS__)
#define ARG_3(x, ...)  ARG_2(__VA_ARGS__)
#define ARG_4(x, ...)  ARG_3(__VA_ARGS__)
#define ARG_5(x, ...)  ARG_4(__VA_ARGS__)
#define ARG_6(x, ...)  ARG_5(__VA_ARGS__)
#define ARG_7(x, ...)  ARG_6(__VA_ARGS__)
#define ARG_8(x, ...)  ARG_7(__VA_ARGS__)
#define ARG_9(x, ...)  ARG_8(__VA_ARGS__)
#define ARG_10(x, ...) ARG_9(__VA_ARGS__)
#define ARG_11(x, ...) ARG_10(__VA_ARGS__)
#define ARG_12(x, ...) ARG_11(__VA_ARGS__)
#define ARG_13(x, ...) ARG_12(__VA_ARGS__)
#define ARG_14(x, ...) ARG_13(__VA_ARGS__)
#define ARG_15(x, ...) ARG_14(__VA_ARGS__)
#define ARG_16(x, ...) ARG_15(__VA_ARGS__)

// If arg == 1, output value, else nothing.
#define IIF_EMPTY(arg, value) IIF ( arg ) (value,)

// Generates a list of nargs elements
// Usage: IIF_LIST(K) (arg1, arg2, arg3... argN) will return arg1, arg2... argK for K <= N
#define IIF_LIST(nargs) CONCAT(IIF_LIST_, nargs)
#define IIF_LIST_1(arg1, ...) arg1
#define IIF_LIST_2(arg1, arg2, ...) arg1, arg2
#define IIF_LIST_3(arg1, arg2, arg3, ...) arg1, arg2, arg3
#define IIF_LIST_4(arg1, arg2, arg3, arg4, ...) arg1, arg2, arg3, arg4

// Checks for NIL
// NIL is used as a placeholder argument for cases where arguments need to be extended by tokens that are expandable (i.e., not ~)
#define IS_NIL(nil) CHECK(NIL_PROBE(nil))

#define NIL_PROBE(nil)          NIL_PROBE_PROXY( CONCAT(nil, _QUERY) ) // wrap
#define NIL_PROBE_PROXY(...)    NIL_PROBE_MERGE(__VA_ARGS__) // expand arguments
#define NIL_PROBE_MERGE(x)      NIL_PROBE_COMBINE_ x // merge
#define NIL_PROBE_COMBINE_(...) PROBE(~) // if merge successful, expand to probe

#define NIL_QUERY ()

// Text formatting for print statements

#define FORMAT_RESET   "\x1B[0m"
#define FORMAT_BOLD    "\x1B[1m"

#define FORMAT_UNBOLD  "\x1B[21m"

#define FORMAT_RED     "\x1B[31m"
#define FORMAT_GREEN   "\x1B[32m"
#define FORMAT_YELLOW  "\x1B[33m"
#define FORMAT_BLUE    "\x1B[34m"
#define FORMAT_MAGENTA "\x1B[35m"
#define FORMAT_CYAN    "\x1B[36m"
#define FORMAT_WHITE   "\x1B[37m"

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

// Some macros need to define stuff in global scope (e.g. TEST() in the unittest framework)
// In typical macro fashion, these should require semicolons at the end
// However, this is not allowed when compiling with -Wpedantic -Werror
// So, we need to invent a semicolon-consuming noop, that's perfectly repeatable.
// Turns out, forward declarations of structs satisfy this requirement.
// Ref: <https://stackoverflow.com/questions/53923706/workaround-for-semicolon-in-global-scope-warning-for-no-op-c-macro>
#define GLOBAL_NOOP struct __GLOBAL_NOOP_SEMICOLON_TRICK

// Primitive Types:
// char, bool, int32_t, int64_t, uint32_t, uint64_t

// Primitive Reference Types:
// These are the ONLY omissions from handling with del, equals, etc.
// Reason: they should NEVER be called as such. del(pointer_t, t) should be more accurately specified, and del(slice_t, t) should use String instead
// slice_t, pointer_t

typedef char* slice_t;
typedef void* pointer_t;

// Primitive Type Classes:
// Character, Boolean, Int32, Int64, UInt32, UInt64

typedef char*     Character;
typedef bool*     Boolean;
typedef int32_t*  Int32;
typedef int64_t*  Int64;
typedef uint32_t* UInt32;
typedef uint64_t* UInt64;

// Primitive Type Handling

// A macro that is defined for all primitive and class types
// It returns PRIMITIVE_TYPE for primitive types, and CLASS_TYPE for classes
#define typeof(cls) IIF( IS_PRIMITIVE_TYPE(cls) ) (PRIMITIVE_TYPE, CLASS_TYPE)

#define PRIMITIVE_TYPE p
#define CLASS_TYPE c

#define IS_PRIMITIVE_TYPE(cls) CHECK(PRIMITIVE_TYPE_PROBE(cls))

#define PRIMITIVE_TYPE_PROBE(cls)          PRIMITIVE_TYPE_PROBE_PROXY( CONCAT(cls, __PRIMITIVE_TYPE) ) // concatenate prefix
#define PRIMITIVE_TYPE_PROBE_PROXY(...)    PRIMITIVE_TYPE_PROBE_MERGE(__VA_ARGS__) // expand arguments
#define PRIMITIVE_TYPE_PROBE_MERGE(x)      PRIMITIVE_TYPE_PROBE_COMBINE_ x // merge
#define PRIMITIVE_TYPE_PROBE_COMBINE_(...) PROBE(~) // if merge successful, expand to probe

#define char__PRIMITIVE_TYPE ()
#define bool__PRIMITIVE_TYPE ()
#define int32_t__PRIMITIVE_TYPE ()
#define int64_t__PRIMITIVE_TYPE ()
#define uint32_t__PRIMITIVE_TYPE ()
#define uint64_t__PRIMITIVE_TYPE ()

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

// Default format string of a primitive type.
#define default_format_string(cls) CONCAT(__default_format_string_, cls)

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

#define __default_format_string_char "%c"
#define __default_format_string_bool "%s"
#define __default_format_string_int32_t "%d"
#define __default_format_string_int64_t "%ld"
#define __default_format_string_uint32_t "%u"
#define __default_format_string_uint64_t "%lu"


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
cls cls ## __new(type value) { cls box = safe_malloc(sizeof(type)); *box = value; return box; } \
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
// They are declared using templates, by the definition of a single Tuple macro:
//
// #define Tuple cls, type1, value1, type2, value2, ... typeN, valueN
// #include "collections/tuple.template.c"
//
// In order to reference individual parts of the 'Tuple' definition, it is reflected into various ARG_N macro invocations via the REFLECT macro
// These are then directly used in the template file

#define TUPLE_ARGS Tuple, NIL, NIL, NIL, NIL, NIL, NIL

// Basic arguments - can determine from the extended TUPLE_ARGS and argument position
#define TUPLE_CLASS  REFLECT(ARG_1, TUPLE_ARGS)
#define TUPLE_TYPE1  REFLECT(ARG_2, TUPLE_ARGS)
#define TUPLE_VALUE1 REFLECT(ARG_3, TUPLE_ARGS)
#define TUPLE_TYPE2  REFLECT(ARG_4, TUPLE_ARGS)
#define TUPLE_VALUE2 REFLECT(ARG_5, TUPLE_ARGS)
#define TUPLE_TYPE3  REFLECT(ARG_6, TUPLE_ARGS)
#define TUPLE_VALUE3 REFLECT(ARG_7, TUPLE_ARGS)
#define TUPLE_TYPE4  REFLECT(ARG_8, TUPLE_ARGS)
#define TUPLE_VALUE4 REFLECT(ARG_9, TUPLE_ARGS)

// Based on the presence of NIL, compute each of arg2, arg3, arg4
#define TUPLE_ARG2 IIF( IS_NIL(TUPLE_TYPE2) )(0, 1)
#define TUPLE_ARG3 IIF( IS_NIL(TUPLE_TYPE3) )(0, 1)
#define TUPLE_ARG4 IIF( IS_NIL(TUPLE_TYPE4) )(0, 1)

// Compute nargs from a reverse expansion of the tuple arguments
#define TUPLE_NARGS_INPUT Tuple, 4, ~, 3, ~, 2, ~, 1
#define TUPLE_NARGS REFLECT(ARG_10, TUPLE_NARGS_INPUT)


// Panics
// These are modeled after Rust's panic!() macro
// Using stringification, they can automatically provide a bit more error context
//
// Additionally, they make use of the C setjmp / longjmp statements for a single, global error handling context
// This is used by unit testing, in order to run sandboxed test code that may panic, and recover from it rather than exiting

#define panic(format_string, args...) __panic("panic", __create_stack_frame, NULL, format_string, ## args)

#define panic_if(condition, format_string, args...) do { \
    if (condition) { \
        __panic("panic_if", __create_stack_frame, "Condition " LITERAL(condition) " is false!", format_string, ## args); \
    } \
} while(0)

#define panic_if_null(pointer, format_string, args...) do { \
    if ((pointer) == NULL) { \
        __panic("panic_if_null", __create_stack_frame, "Pointer " LITERAL(pointer) " is NULL!", format_string, ## args); \
    } \
} while (0)


// Panic Recovery Global Context
extern bool    __panic_recovery_active;
extern jmp_buf __panic_recovery_context;

struct StackFrame__struct {
    slice_t func;
    slice_t file;
    uint32_t line;
};

typedef struct StackFrame__struct StackFrame;

#define MAX_STACK 16

// Stack Tracing
extern StackFrame __stack_frame_array[MAX_STACK];
extern uint32_t   __stack_frame_length;
extern bool       __panic_invoked;

// Internal function, like exit(), which tries to invoke a panic
// If the global recovery context is set, uses longjmp to return to there
void __panic(slice_t panic_func, StackFrame frame_in, slice_t detail_string, slice_t format_string, ...);

// Internal functions for pushing a popping from stack frames
// Any functions which want to store stack frames for error cases should call these
// Calling: foo(..., __create_stack_frame);
// Declaring:
// foo(..., StackFrame frame)
// {
//     __stack_frame_push(frame);
//     ...
//     __stack_frame_pop();
// }
void __stack_frame_push(StackFrame frame);
void __stack_frame_pop();

#define __create_stack_frame ((StackFrame) { __func__, __FILE__, __LINE__ })


#define try do {\
    __panic_recovery_active = true; \
    if (!setjmp(__panic_recovery_context))
#define catch \
    else
#define finally \
    __panic_recovery_active = false; \
} while (0)


// Misc. Features

// Standard Memory Allocations
#define class_malloc(cls) __malloc(sizeof(struct CONCAT(cls, __struct)), __create_stack_frame)
#define safe_malloc(size) __malloc(size, __create_stack_frame)
#define safe_realloc(ptr, size) __realloc((pointer_t*) (& (ptr)), size, __create_stack_frame)

// Calls malloc() guarded with an panic. Return value is gaurenteed to be non null
pointer_t __malloc(uint64_t size, StackFrame frame);

// Calls realloc() guarded with a panic. ref_ptr is gaurenteed to be non null on return.
void __realloc(pointer_t* ref_ptr, uint64_t size, StackFrame frame);

// Rust 'loop' keyword
#define loop while (true)

// Move Semantics

#define move(ptr) (__move((pointer_t*)(& (ptr))))
pointer_t __move(pointer_t* ref_ptr);


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
#include "collections/result.h"
#include "collections/set.h"

#endif