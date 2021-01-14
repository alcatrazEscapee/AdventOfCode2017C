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

```c
new(T, args...)
// cls is the class being constructed
// args... are specific arguments for that classes constructor.
```

A constructor is a method declared as:

```c
T T__new(args...);
```

Constructors make heavy use of standard `lib.h` methods for memory allocations:

```c
// Allocates memory for a Class. Panics if it cannot allocate memory 
pointer_T class_malloc(Class cls);

// Allocates memory for a named, sized amount. Used for sub-elements such as the slice_t of a String. Panics if it cannot allocate memory.
pointer_t safe_malloc(Name name, uint32_t size);

// Guarded call to realloc() which reallocates ptr up to new_size. Panics if it cannot allocate memory.
void safe_realloc(Name name, pointer_t ptr, uint32_t new_size);
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

- A pointer is passed by value (`T t`) to a function 
- The function takes ownership of the pointer, and it IS NOT valid to use it after passing it, or call `del(T, t)`.
- Examples include `del(T, t)`, which takes ownership of 't'

You can only call move() when given a *Mutable Reference* (which must be unique). Then, the original value is nullified, there are no other references, and you now have ownership of the reference. A *Shared Reference* cannot be upgraded into ownership, but a *Mutable Reference* can be.

For example, an iterator through an `ArrayList<String>` exposes a *Shared Reference*. There are two references (the list, owner, and the iterator, borrowing) to the value

Another example, the `StringLines` iterator owns each string instance, and the loop code is given a *Mutable Reference*. It can take ownership through calling `move()`.

## Iterators

Iterators

## Panics

Panics

### Error Handling

Error handling: `try` / `catch` / `finally` statements

## Collections

Library collection types. Templates and standard types.

### ArrayList

### PrimitiveArrayList

### Map

### Set

### Tuples

Tuples are template based classes which are defined for primitive and class types. They abstract a lot of boilerplate code away from defining the notions of a class on heterogeneous collections of data. They can (currently) hold up to four members. They are fully defined classes with elementwise equality, construction, and destruction.

A tuple is defined via two statements: A definition of the tuple arguments, and the inclusion of the tuple template file:

```c
#define Tuple IntStringPair, int32_t, id, String, name
#include "lib/collections/tuple.template.c"
```

The definition of `Tuple` must consist of `ClassName, (MemberType, MemberName)+`, for up to four member type-name pairs. For direct inclusion in a particular source file, the `.template.c` file can be included, or for a generic tuple that is used outside a single source file, the `.template.h` file can be included.
