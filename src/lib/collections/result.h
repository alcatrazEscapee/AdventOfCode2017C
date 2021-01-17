// An implementation of an optional result type
// Based on a combination of Rust's Result<T, E> { Ok(T), Err(E) } and Option<T> { Some(T), None } structs
// Implemented specifically for primitive types, and with pointer_t for classes

#include "../lib.h"

#ifndef COLLECTIONS_RESULT_H
#define COLLECTIONS_RESULT_H

typedef enum { _Err = 0, _Ok = 1 } __Result;

#define RESULT_GENERIC_PREFIX(result, method) _Generic((result), \
    struct Result_char__struct : CONCAT(result_char_, method), \
    struct Result_bool__struct : CONCAT(result_bool_, method), \
    struct Result_int32_t__struct : CONCAT(result_int32_t_, method), \
    struct Result_int64_t__struct : CONCAT(result_int64_t_, method), \
    struct Result_uint32_t__struct : CONCAT(result_uint32_t_, method), \
    struct Result_uint64_t__struct : CONCAT(result_uint64_t_, method) \
)

#define RESULT_STRUCT(T) struct { __Result type; T value; }
#define RESULT_VALUE(state, value) ((RESULT_CAST(value) { state, value }))
#define RESULT_CAST(value) _Generic((value), \
    bool: Result_bool, \
    char: Result_char, \
    int32_t: Result_int32_t, \
    uint32_t: Result_uint32_t, \
    int64_t: Result_int64_t, \
    uint64_t: Result_uint64_t \
)

// Type
#define Result(T) CONCAT(Result_, typeof(T)) (T)
#define Result_p(T) CONCAT(Result_, T)
#define Result_c(T) Result_pointer_t

// Constructions
#define Ok(T, value) ((Result(T)) { _Ok, value })
#define Err(T) ((Result(T)) {_Err, default_value(T)})

// Implemented Methods

#define is_ok(result) ((result).type == _Ok)
#define is_err(result) ((result).type == _Err) 

// Panics if Err()
#define unwrap(args...) UNROLL_UNWRAP(args)

// Allows Err()
#define unwrap_or(args...) UNROLL_UNWRAP_OR(args)
#define unwrap_default(result) ((result).value)

// Handling both primitive and class variants for unwrap() and unwrap_or()
#define UNROLL_UNWRAP(args...) UNROLL_REFLECT_UNWRAP(args, UNWRAP_GENERIC, UNWRAP_PRIMITIVE) (args)
#define UNROLL_REFLECT_UNWRAP(args...) ARG_3(args)
#define UNWRAP_GENERIC(cls, result) ((cls) result_pointer_t_unwrap (result, class(cls)))
#define UNWRAP_PRIMITIVE(result) RESULT_GENERIC_PREFIX(result, unwrap) (result)

#define UNROLL_UNWRAP_OR(args...) UNROLL_REFLECT_UNWRAP_OR(args, UNWRAP_OR_GENERIC, UNWRAP_OR_PRIMITIVE) (args)
#define UNROLL_REFLECT_UNWRAP_OR(args...) ARG_4(args)
#define UNWRAP_OR_GENERIC(cls, result, value) ((cls) result_pointer_t_unwrap_or (result, value, class(cls)))
#define UNWRAP_OR_PRIMITIVE(result, value) RESULT_GENERIC_PREFIX(result, unwrap_or) (result, value)

#define declare_result(T) \
struct CONCAT3(Result_, T, __struct) { __Result type; T value; }; \
typedef struct CONCAT3(Result_, T, __struct) Result(T); \
T CONCAT3(result_, T, _unwrap) (Result(T) result); \
T CONCAT3(result_, T, _unwrap_or) (Result(T) result, T value)

#define impl_result(T) \
T CONCAT3(result_, T, _unwrap) (Result(T) result) \
{ \
    panic_if(is_err(result), "unwrap() called on Result(Err)"); \
    return result.value; \
} \
\
T CONCAT3(result_, T, _unwrap_or) (Result(T) result, T value) \
{ \
    return is_ok(result) ? result.value : value; \
}

// Declare Result(T) for all primitive types and pointer_t

declare_result(bool);
declare_result(char);
declare_result(int32_t);
declare_result(uint32_t);
declare_result(int64_t);
declare_result(uint64_t);

// pointer_t is different, because it needs to take the Class<T> as a param
typedef RESULT_STRUCT(pointer_t) Result(pointer_t);

pointer_t CONCAT3(result_, pointer_t, _unwrap) (Result(pointer_t) result, Class cls);
pointer_t CONCAT3(result_, pointer_t, _unwrap_or) (Result(pointer_t) result, pointer_t value, Class cls);


#endif