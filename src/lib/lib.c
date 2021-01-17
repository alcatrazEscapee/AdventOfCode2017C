
#include "lib.h"

// Panics

bool    __panic_recovery_active = false;
jmp_buf __panic_recovery_context;

void __panic(slice_t panic_string, const char * func_string, slice_t extra_panic_string, slice_t format_string, ...)
{
    println("\n" FORMAT_BOLD FORMAT_RED "Program Panicked!" FORMAT_RESET); // Header for all panics
    if (extra_panic_string != NULL)
    {
        println(extra_panic_string); // Extra error, if using panic_if or panic_if_null
    }

    va_list arg;
    va_start(arg, format_string);
    vprintf(format_string, arg); // Print the panic-supplied error
    va_end(arg);

    printf("\n"); // Newline, as the previous needs to use varadic printf, not println
    println(panic_string, func_string); // Trace + function. Format string is required to contain one '%s' for the function name as that cannot be macro expanded to a literal

    // Actually execute the panic
    if (__panic_recovery_active)
    {
        // Recovery is possible. Jump directly to recovery location
        longjmp(__panic_recovery_context, 1);
    }
    else
    {
        // Abort!
        exit(1);
    }
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