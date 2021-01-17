
#include "lib.h"

// Panics
bool    __panic_recovery_active = false;
jmp_buf __panic_recovery_context;

// Stack Tracing
StackFrame __stack_frame_array[MAX_STACK];
uint32_t   __stack_frame_length = 0;
bool       __panic_invoked = false;

void __panic(slice_t panic_func, StackFrame frame_in, slice_t detail_string, slice_t format_string, ...)
{
    __panic_invoked = true;  // First, set the global panic invocation to true. This prevents re-entrancy when dealing with stack frames
    __stack_frame_push(frame_in); // Push the provided stack frame. The stack is maintained to always have space for this one.

    println("\n" FORMAT_BOLD FORMAT_RED "Program Panicked!" FORMAT_RESET); // Header for all panics
    if (detail_string != NULL)
    {
        println(detail_string); // Extra error, if using panic_if or panic_if_null
    }

    va_list arg;
    va_start(arg, format_string);
    vprintf(format_string, arg); // Print the panic-supplied error
    va_end(arg);

    printf("\n"); // Newline, as the previous needs to use varadic printf, not println

    // Print the stack trace, walking backwards along the stack frames
    // When printing, the line + file are offset from the function names. For instance, the stack looking like:
    // [ { foo(), file.c, 10 }, { baz(), file.c, 30 }]
    // There is a 'partial' stack frame, { panic(), ??, ?? } appended to the top, and all file + line numbers are shifted up
    // That way, line + file numbers match the CALL site of the function, not the declaration of it.

    // Print the partial stack frame
    StackFrame prev;
    StackFrame frame = __stack_frame_array[__stack_frame_length - 1];
    printf("  at: " FORMAT_BOLD FORMAT_CYAN "%s" FORMAT_RESET "(...) from " FORMAT_BOLD FORMAT_WHITE "%s:%d" FORMAT_RESET "\n", panic_func, frame.file, frame.line);

    // Additional full stack frames
    for (uint32_t i = __stack_frame_length - 1; i > 0; i--)
    {
        prev = frame;
        frame = __stack_frame_array[i - 1];
        printf("  at: " FORMAT_BOLD FORMAT_CYAN "%s" FORMAT_RESET "(...) from " FORMAT_BOLD FORMAT_WHITE "%s:%d" FORMAT_RESET "\n", prev.func, frame.file, frame.line);
    }

    // Print the final frame, which does not have a line number. The file is assumed to be the same as the last stack frame
    printf("  at: " FORMAT_BOLD FORMAT_CYAN "%s" FORMAT_RESET "(...) from " FORMAT_BOLD FORMAT_WHITE "%s:?" FORMAT_RESET "\n", frame.func, frame.file);

    // Actually execute the panic
    if (__panic_recovery_active)
    {
        // Recovery is possible.
        __stack_frame_length = 0; // Nuke the stack
        longjmp(__panic_recovery_context, 1); // Jump to recovery location
    }
    else
    {
        // Abort!
        exit(1);
    }
}

void __stack_frame_push(StackFrame frame)
{
    __stack_frame_array[__stack_frame_length] = frame;
    __stack_frame_length++;
    if (!__panic_invoked && __stack_frame_length == MAX_STACK - 1)
    {
        panic("Stack Overflow!");
    }
}

void __stack_frame_pop()
{
    if (__stack_frame_length == 0)
    {
        panic("Stack Underflow!");
    }
    __stack_frame_length--;
}

// Implementations of Primitive Box Classes

impl_primitive_class(Character, char);
impl_primitive_class(Boolean, bool);
impl_primitive_class(Int32, int32_t);
impl_primitive_class(Int64, int64_t);
impl_primitive_class(UInt32, uint32_t);
impl_primitive_class(UInt64, uint64_t);

// Implementation of Class and Void

impl_class(Class);

void Class__del(Class cls) {}
Class Class__copy(Class cls) { return cls; }
bool Class__equals(Class left, Class right) { return left == right; }

int32_t Class__compare(Class left, Class right)
{
    panic("compare() is not defined for class Class"); 
    return 0;
}

uint32_t Class__hash(Class cls)
{
    // Hash the name as a string
    uint32_t h = 0;
    for (uint32_t i = 0; cls->name[i] != '\0'; i++)
    {
        h = (h * 31) + (uint32_t) cls->name[i];
    }
    return h;
}

String Class__format(Class cls)
{
    String s = new(String, "Class<");
    str_append_slice(s, cls->name);
    str_append_char(s, '>');
    return s;
}

impl_class(Void);

void Void__del(Void v) {}
Void Void__copy(Void v) { return NULL; }
bool Void__equals(Void left, Void right) { return true; }
int32_t Void__compare(Void left, Void right) { return 0; }
uint32_t Void__hash(Void v) { return 0; }
String Void__format(Void v) { return new(String, "Void"); }


// Class Methods - Passthrough Functions

void __del(pointer_t instance, FnDel f)
{
    if (instance != NULL)
    {
        f(instance);
    }
}

pointer_t __copy(pointer_t instance, FnCopy f)
{
    if (instance != NULL)
    {
        return f(instance);
    }
    return NULL;
}

bool __equals(pointer_t left, pointer_t right, FnEquals f)
{
    if (left == right)
    {
        return true;
    }
    if (left == NULL || right == NULL)
    {
        return false;
    }
    return f(left, right);
}

int32_t __compare(pointer_t left, pointer_t right, FnCompare f)
{
    panic_if(left == NULL, "compare() had null left argument");
    panic_if(right == NULL, "compare() had null right argument");
    return f(left, right);
}

uint32_t __hash(pointer_t instance, FnHash f)
{
    if (instance != NULL)
    {
        return f(instance);
    }
    return 0;
}

String __format(pointer_t instance, FnFormat f)
{
    if (instance != NULL)
    {
        return f(instance);
    }
    return new(String, "NULL");
}

// Safe allocation
pointer_t __malloc(slice_t name, uint32_t size)
{
    pointer_t ptr = malloc(size);
    panic_if_null(ptr, "Out of Memory: Cannot allocate %d bytes for %s", size, name);
    return ptr;
}

void __realloc(slice_t name, pointer_t* ref_ptr, uint32_t size)
{
    pointer_t realloc_ptr = realloc(*ref_ptr, size);
    panic_if_null(realloc_ptr, "Out of Memory: Cannot allocate %d bytes for %s", size, name);
    *ref_ptr = realloc_ptr;
}

// Move Semantics
pointer_t __move(pointer_t* ref_ptr)
{
    pointer_t moved_ptr = *ref_ptr;
    *ref_ptr = NULL;
    return moved_ptr;
}