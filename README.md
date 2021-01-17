# Advent Of Code 2017 In C

This is a repository for my Advent of Code 2017 solutions, all written in absolute bare-bones C, in order to gain experience and practice with low level code, memory management, and creating my own infrastructure and data structures. In solving these puzzles I have *completely* overengineered my solutions to the point of creating my own superset of C with the root of all evil in programming languages: macros.

The below documentation serves to outline the structure of the pseudo-language I have created in order to avoid writing idiomatic C or C++.

A series of extensions for C, using a whole host of macros and other horrible language features, to bring in features from other languages.

## Types

Primitive Types:

- `bool` : A boolean
- `char` : A 8 bit ASCII character
- `int32_t` : A 32 bit integer
- `int64_t` : A 64 bit integer
- `uint32_t` : An unsigned 32 bit integer
- `uint64_t` : An unsigned 64 bit integer

Primitive Reference Types:

- `slice_t` : A static string literal (`char *`)
- `pointer_t` : A pointer to an arbitrary or generic value (`void *`)

Standard Library Classes:

- `Boolean` : A heap allocated boolean.
- `Character` : A heap allocated 8 bit ASCII character
- `Int32` : A heap allocated 32 bit integer
- `Int64` : A heap allocated 64 bit integer
- `UInt32` : A heap allocated unsigned 32 bit integer
- `UInt64` : A heap allocated unsigned 64 bit integer
- `String` : A heap allocated string type.
- `Void` : The class of `void` / `NULL`
- `Class` : A meta object representing the type of another set of objects. For any class `T`, `class(T)` is an object of type `Class<T>`

**Note**: When referring to classes, there are two different things which may be referred to:

- A `Class` specifically refers to the *name* of a class, such as `Boolean`, `Int32`, or `String`. It is the *type* of something.
- A `Class<T>` specified with a generic `T`, refers to the *class for a type T*, which is an object of type `Class`. It is the result of the statement `class(T)`.

Consider the following snippet:

```c
T t = new(T); // T is a Class, t is a T.
Class ct = class(T); // Class is a Class, ct is a Class<T>.
```

## Classes

Classes are fundamentally just pointers to structs. However, in order to be able to use classes generically, various macro and other functions exist in order to allow common functionality, such as basic memory management (`new` / `del` / `copy`), use in hash based collections (`hash` / `equals`), or sorting (`compare`).

### Constructors

The constructor of a class creates and returns a new instance of the provided class.

```cpp
new(T, args...)
// cls is the class being constructed
// args... are specific arguments for that classes constructor.
```

A constructor is a method declared as:

```c
T T__new(args...);
```

Constructors make heavy use of standard `lib.h` methods for memory allocations:

```cpp
// Allocates memory for a Class. Panics if it cannot allocate memory 
pointer_t class_malloc(Class cls);

// Allocates memory for a named, sized amount. Used for sub-elements such as the slice_t of a String. Panics if it cannot allocate memory.
pointer_t safe_malloc(uint32_t size);

// Guarded call to realloc() which reallocates ptr up to new_size. Panics if it cannot allocate memory.
void safe_realloc(pointer_t ptr, uint32_t new_size);
```

Some classes may also support a `copy` constructor, which returns an new instance that is equal to the provided one. It's signature is:

```c
T copy(Class cls, T t);
```

It can be declared as:

```c
T T__copy(T t);
```

The copy constructor:

- MUST return a new instance of the class.
- MUST NOT modify the passed in instance in any way.
- The following MUST always be true: `equals(T, copy(T, t), t)`

#### Destructors

A destructor is a method which is invoked to clean up the instance of a class.

```c
del(Class cls, T instance);
del_c(Class<T> cls, T instance);
```

#### Class Methods

Any class must also define three other methods: `equals`, `hash`, and `compare`.

```c
// Returns true if left == right, false otherwise.
// NULL == NULL, and NULL != Everything else by definition
// Must be an equivilance relation.
bool equals(Class cls, T left, T right);
bool equals_c(Class<T> cls, T left, T right);

// Defines an ordering on two instances.
// NULL is considered un-orderable and any call of compare() on a NULL will panic
// Returns 1 if left > right, -1 if left < right, and 0 if left == right
// If the instances are un-orderable, this may panic.
int32_t compare(Class cls, T left, T right);
int32_t compare_c(Class<T> cls, T left, T right);

// Computes a hash for an instance
// The following relation must hold: equals(T, a, b) => hash(T, a) == hash(T, b)
uint32_t hash(Class cls, T instance);
uint32_t hash_c(Class<T> cls, T instance);
```

In addition to the above, there are variants of `compare` for all standard relations that can be used by the caller:

```c
// Computes if left <, >, <=, >= right, respectively.
bool compare_[lt|gt|le|ge](Class cls, T left, T right);
bool compare_[lt|gt|le|ge]_c(Class<T> cls, T left, T right);
```

In order to declare any of the following methods, the following is required. In all cases, the instances are required to be non-null. (Even though the contract for `equals` and `hash` permits null values, they are not passed through to the definitions of these functions).

```c
bool T__equals(T left, T right);
int32_t T__compare(T left, T right);
uint32_t T__hash(T instance);
```

### Move / Ownership Semantics

Most functions have specific contracts about how they manage ownership of pointers. These are a few notions, simplified from C++ / C practices, and Rust:

#### Borrowing

- A class is passed by value (`T t`) to a function.
- The function MUST NOT call `del(T, t)` during it's execution, and at the return of the function, the pointer must still be valid

#### Moving

- A class is passed by reference (`T* t`) to a function.
- The pointer may be destroyed at the end of this function. (e.g. the function called either `del(T, *t)` or `v = move(T, *t))`

This is heavily context dependent. It may apply, e.g. to iterator methods. In the body of an iterator, the main code MAY take ownership of the internal value by calling `move(T, &t)`. This value will then by nullified (and the required call to `del(T, t)` will noop)

#### Destroying

- A pointer is passed by value (`T t`) to a function.
- The function takes ownership of the pointer, and it IS NOT valid to use it after passing it, or call `del(T, t)`.
- Examples include `del(T, t)`, which takes ownership of 't'

You can only call move() when given a *Mutable Reference* (which must be unique). Then, the original value is nullified, there are no other references, and you now have ownership of the reference. A *Shared Reference* cannot be upgraded into ownership, but a *Mutable Reference* can be.

For example, an iterator through an `ArrayList<String>` exposes a *Shared Reference*. There are two references (the list, owner, and the iterator, borrowing) to the value

Another example, the `StringLines` iterator owns each string instance, and the loop code is given a *Mutable Reference*. It can take ownership through calling `move()`.

## Iterators

An `Iterator<T>` and the `iter` statement are wrappers for iterating over collections in which the iteration method is non-standard. They are macros which replace the body of a `for (...)` statement, and obey a common contract of how the iterator behaves.

An `Iterator<T>` is a struct which contains at least the following:

```c
typedef struct {
    T value;
} Iterator(T);
```

The arguments to an `iter` statement will depend on the class of the object being iterated, but in general all `iter()` statements have at least the following arguments:

- `Class cls` : This is the class name (or method name) that is being iterated. This can be a class which supports iteration (such as `String`, or `ArrayList`), or it can be a specific iterator method name (such as `StringSplit`).
- `Iterator(T) it` : This is the variable name for the iterator that is declared within the loop.
- `T collection` : This is the object that is being iterated over.

Additional arguments may be required as per the individual iterators (for example, `StringSplit` requires an additional argument `slice_t delim`, which is the characters to split the input string on). For example:

```cpp
String s = new(String, "abc");
for iter(String, it, s) {
    printf("'%c', ", it.value);
}
// Prints:
// 'a', 'b', 'c', 
```

There is a specialization of iterators for generic collections. For example, iteration through an `ArrayList<T>` will have an iterator with a `pointer_t value` for each element. In the case that a specific type is known, the `type_iter()` statement can be used instead. This wraps a standard iterator call with a generically specialized one, using an anonymous struct declaration. It requires one additional argument, immediately after the class name:

- `Type type` : The type of the `value` variable to be declared.

The `Iterator(T)` exposed by the `type_iter()` statement is a declaration of the following struct:

```c
struct {
    Iterator(T) parent;
    Type value;
}
```

It can be used as such:

```cpp
ArrayList list = /* Suppose this is an ArrayList<String> */

for iter(ArrayList, it, list) {
    // it.value is typed pointer_t, and this cast is required
    println("Element: %s", ((String)it.value)->slice);
}

for type_iter(ArrayList, String, it, list) {
    // it.value is typed String, and no cast is required here
    println("Element: %s", it.value->slice);
}
```

Iterators are declared through three methods, often implemented as macros. These are:

```cpp
// Returns a struct initializer for a given iterator
Iterator(T) T__iterator__start(args...);

// Tests the iterator if it has reached the end of the collection
bool T__iterator__test(Iterator(T)* it, args...);

// Advances the iterator to the next element in the collection
void T__iterator__next(Iterator(T)* it, args...);
```

These are composed in (roughly) the following manner via the expansion of the `iter` macro:

```cpp
for (Iterator(T) it = T__iterator__start(...); T__iterator__test(&it, ...); T__iterator__next(&it, ...))
```

## Error Handling

The style of error handling and recovery is based on principles from Rust more so than C style (return codes from functions). As such, there are three main mechanisms for error reporting and handling:

- Panics (The name and idea borrowed from Rust's `panic!()` macro), are invoked in *unrecoverable* situations. These are errors such as Out of Memory or Index Out of Bounds. The implementation of `panic`, `panic_if`, and `panic_if_null` are designed to be as clear as possible about where the error occurred (including debugging information such as the current file and line number, and function), and the reason for failure. They are designed to support what should be program level assertions.
- For situations where expectable errors may occur, and need to be communicated back to the caller, the `Result<T>` type should be used. This is a simple struct which can either store an `Ok(Class cls, T t)` value, or `Err(Class cls)`. It is similar in implementation to Rust's `Option<T>`. It has a series of `unwrap` methods and variants to handle all primitive or class types.
- Finally, in the rare case that it would be advisable to prevent a `panic()` invocation from exiting the program (for example, in the unit test framework, when tests are ideally ran in isolation from each other), `try` / `catch` blocks can be used to recover from a `panic()`. They have several limitations, and due to the lack of cleanup / stack backtracking, they can very easily lead to memory leaks. If a panic occurs, it is an error state in the program, `try` and `catch` simply provide a recovery and continuation mechanism.

In order to provide detailed trace-back information in the case of a panic, a primitive, finite exception stack trace mechanism is used. This uses a global call stack, which is appended to with static information (such as `__FILE__`, `__LINE__`, and `__func__`) at specific call sites. When a panic is invoked, the stack is popped to print the entire context available.

In order to declare a function which records a stack frame, it must take a `StackFrame` parameter. Callers should supply `__create_stack_frame` as an argument (this is typically implemented with macro redirects to a function, for example, `__malloc` or `Result<T>`s `unwrap` function). Inside the function, it should call both `__stack_frame_push` and `__stack_frame_pop`:

```cpp
void function_with_stack_frame(..., StackFrame frame)
{
    __stack_frame_push(frame);
    ...
    __stack_frame_pop();
}
```

Panic macros come in three flavors:

```c
// Unconditional panic. Arguments are as to printf
void panic(slice_t format_string, ...);

// Conditional panic. Arguments are as to printf. Use preferrentially over if (x) { panic() }
void panic_if(bool condition, slice_t format_string, ...);

// Conditional panic. Arguments are as to printf. Use preferrentially over if (x != NULL) panic()
void panic_if_null(T left, T right, slice_t format_string, ...);
```

Panics internally invoke the `__panic()` function, supplying a series of context strings, allowing for a detailed trace to be printed to the console. When they are invoked, they will either (depending if the global context for `try` / `catch` handling is set), invoke `longjmp`, or `exit(1)` to return to the `catch` block, or terminate execution of the program.

Try / Catch statements are a very primitive implementation of panic recovery. They use a global context to record the current try context.

- Due to a lack of cleanup abilities (such as pointers dropping out of scope and being cleaned up) that are present in Rust or C++, this very often will lead to memory leaks if a panic is invoked.
- A `try` block cannot be returned from within, as this will leak the try context.
- In the event the `catch` block is reached, there is no introspection of the specific panic that was hit. The panic will always be printed to the console, and it cannot be checked at this time.

That said, `try catch` statements can be used in the following manner:

```cpp
try
{
    // code that may invoke panic()
    // if it does, the rest of the try block will be skipped and execution will transfer to the catch block
}
catch
{
    // will be executed ONLY if the try block paniced
}
finally;
```

## Collections

Library collection types. Templates and standard types.

### ArrayList

A basic auto-resizable, array backed list. It supports generic `pointer_t` element values, and methods that are generic for any `ArrayList` or `PrimitiveArrayList<T>` implementation.

### PrimitiveArrayList

A type-specific auto-resizable array backed list. These are implemented through template files for every primitive type. The type is `PrimitiveArrayList(type)`. This is more efficient that using boxed / heap allocated class types such as `ArrayList<Int32>`.

### Map

A hash based key-value pair map. It stores values densely in two backing arrays, and uses linear probing for `O(1)` access, avoiding excessive indirection e.g. through a bucket / linked list map implementation.

### Set

This is a hash based set, with `O(1)` contains checks. It is a simplified implementation of the `Map` class, having no unnecessary values array or the `Void` class.

### Result

Result is a cross between an `Optional<T>` and Rust's `Result<T, E>` type. It is implemented for `pointer_t`, and all primitive types. It has various `unwrap` based methods that allow querying of the result's state, and either panicking or defaulting in error states. A `Result` is typed with `Result(T)`, where `T` is the type of the underlying value. A result can be created either with `Ok(T, value)` or `Err(T)`.

It is used, for example, as the return type for the `str_parse()` method:

```cpp
// Parses a primitive type from the front of a string.
Result(T) str_parse(Class T, String string);
```

Which can then be used to indicate errors in parsing:

```cpp
Result(int32_t) int_result = str_parse(int32_t, "123"); // This will return Ok(int32_t, 123)

unwrap(int_result); // Returns 123
is_ok(int_result); // Returns true
is_err(int_result); // Returns false

Result(int32_t) int_result = str_parse(int32_t, "not an int"); // This will return Err(int32_t)

unwrap(int_result); // This will panic
unwrap_or(int_result, 123); // Returns 123
unwrap_default(int_result); // Returns default_value(int32_t), aka 0
```

### Tuples

Tuples are template based classes which are defined for primitive and class types. They abstract a lot of boilerplate code away from defining the notions of a class on heterogeneous collections of data. They can (currently) hold up to four members. They are fully defined classes with elementwise equality, construction, and destruction.

A tuple is defined via two statements: A definition of the tuple arguments, and the inclusion of the tuple template file:

```c
#define Tuple IntStringPair, int32_t, id, String, name
#include "lib/collections/tuple.template.c"
```

The definition of `Tuple` must consist of `ClassName, (MemberType, MemberName)+`, for up to four member type-name pairs. For direct inclusion in a particular source file, the `.template.c` file can be included, or for a generic tuple that is used outside a single source file, the `.template.h` file can be included.
