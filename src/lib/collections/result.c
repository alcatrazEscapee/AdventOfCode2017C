#include "result.h"

// Implement unwrap() and unwrap_or() for all Result(T) types

impl_result(bool);
impl_result(char);
impl_result(int32_t);
impl_result(uint32_t);
impl_result(int64_t);
impl_result(uint64_t);

// Result(pointer_t) needs to have the additional Class<T> param

pointer_t CONCAT3(result_, pointer_t, _unwrap) (Result(pointer_t) result, Class cls, StackFrame frame)
{
    __stack_frame_push(frame);
    panic_if(is_err(result), "unwrap() called on Result<%s>", cls->name);
    __stack_frame_pop();
    return result.value;
}

pointer_t CONCAT3(result_, pointer_t, _unwrap_or) (Result(pointer_t) result, pointer_t value, Class cls)
{
    if (is_ok(result))
    {
        del_c(cls, value);
        return result.value;
    }
    return value;
}


