#include "set.h"

#define LOAD_FACTOR 0.75

// Private Methods

static void set_rehash(Set set);

Set Set__new(uint32_t initial_size, Class value_class)
{
    initial_size = next_highest_power_of_two(initial_size);
    Set set = class_malloc(Set);

    set->values = safe_malloc(sizeof(pointer_t) * initial_size);
    set->value_class = value_class;
    set->size = initial_size;
    set->length = 0;

    for (uint32_t i = 0; i < initial_size; i++)
    {
        set->values[i] = NULL;
    }

    return set;
}

void Set__del(Set set)
{
    for iter(Set, it, set)
    {
        del_c(set->value_class, it.value);
    }
    free(set->values);
    free(set);
}

String Set__format(Set set)
{
    String s = new(String, "Set<");
    str_append(s, set->value_class->name);
    str_append(s, ">{");
    if (set->length == 0)
    {
        str_append(s, "}");
        return s;
    }
    else
    {
        for iter(Set, it, set)
        {
            str_append(s, format_c(set->value_class, it.value));
            str_append(s, ", ");
        }
    }
    str_pop(s, 2); // Pop the last ', '
    str_append(s, "}");
    return s;
}


// Iterator

bool Set__iterator__test(Iterator(Set)* it, Set set)
{
    for (;it->index < set->size; it->index++) // Don't iterate off the end of the set
    {
        pointer_t value = set->values[it->index]; // Stop at valid locations
        if (value != NULL)
        {
            it->value = value;
            return true;
        }
    }
    return false;
}


// Instance Methods

bool set_put(Set set, pointer_t value)
{
    panic_if_null(value, "Null Pointer: Set value must not be null");

    // In order to gaurentee proper set function, the set must have always at least one empty entry
    // This means we need to re-hash the set when inserting at specific sizes
    if (set->length + 1 >= (uint32_t) (LOAD_FACTOR * set->size))
    {
        set_rehash(set);
    }

    uint32_t mask = (set->size - 1);
    uint32_t index = hash_c(set->value_class, value) & mask;
    pointer_t current_value = set->values[index];

    while (current_value != NULL)
    {
        // Test current value
        if (equals_c(set->value_class, current_value, value))
        {
            // Value match. Replace the value at this index.
            // Delete the original value, keep the queried value intact. Ownership is given to the set (but borrowed in return)
            del_c(set->value_class, set->values[index]);
            set->values[index] = value;
            return true; // Previous value found
        }
        index = (index + 1) & mask;
        current_value = set->values[index];
    }
    // Found a null value position before a match, there was no matching element
    set->values[index] = value;
    set->length++;
    return false;
}

bool set_contains(Set set, pointer_t value)
{
    panic_if_null(value, "Null Pointer: Set value must not be null.");

    uint32_t mask = (set->size - 1);
    uint32_t index = hash_c(set->value_class, value) & mask;
    pointer_t current_value = set->values[index];
    
    while (current_value != NULL) // The set must always have at least one empty spot - so this is gaurenteed to terminate
    {
        // Test current value
        if (equals_c(set->value_class, current_value, value))
        {
            return true; // Value match
        }
        index = (index + 1) & mask;
        current_value = set->values[index];
    }
    return false; // No match
}

void set_clear(Set set)
{
    for iter(Set, it, set)
    {
        del_c(set->value_class, it.value);
        set->values[it.index] = NULL;
    }
    set->length = 0;
}


// Private Methods

static void set_rehash(Set set)
{
    // Save a reference to the existing values array
    pointer_t* old_values = set->values;
    uint32_t old_size = set->size;
    uint32_t new_size = old_size << 1;

    // Reallocate the new array
    set->values = safe_malloc(sizeof(pointer_t) * new_size);

    for (uint32_t i = 0; i < new_size; i++)
    {
        set->values[i] = NULL;
    }

    // Set the set to empty
    set->size = new_size;
    set->length = 0;

    // Insert all old values
    for (uint32_t index = 0; index < old_size; index++)
    {
        pointer_t old_value = old_values[index];
        if (old_value != NULL)
        {
            set_put(set, old_value);
        }
    }

    // Free now-unused old values array
    free(old_values);
}