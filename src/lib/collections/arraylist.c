// Generic ArrayList

#include "arraylist.h"

// Primitive Array List Implementations
// Uses Templating to achieve similar classes with proper line number references

#define type char
#include "primitivearraylist.template.c"

#define type bool
#include "primitivearraylist.template.c"

#define type int32_t
#include "primitivearraylist.template.c"

#define type int64_t
#include "primitivearraylist.template.c"

#define type uint32_t
#include "primitivearraylist.template.c"

#define type uint64_t
#include "primitivearraylist.template.c"

// ArrayList Class

// Constructor
ArrayList ArrayList__new(uint32_t initial_size, Class value_class)
{
    ArrayList list = class_malloc(ArrayList);
    
    list->values = safe_malloc(sizeof(pointer_t) * initial_size);
    list->value_class = value_class;
    list->size = initial_size;
    list->length = 0;

    return list;
}

// Destructor
void ArrayList__del(ArrayList list)
{
    al_clear(list);
    free(list->values);
    free(list);
}

ArrayList ArrayList__copy(ArrayList list)
{
    ArrayList new_list = new(ArrayList, list->length, list->value_class);
    for iter(ArrayList, it, list)
    {
        new_list->values[it.index] = copy_c(list->value_class, it.value);
    }
    new_list->length = list->length;
    return new_list;
}

String ArrayList__format(ArrayList list)
{
    String s = new(String, "ArrayList{");
    if (list->length == 0)
    {
        str_append(s, "}");
        return s;
    }
    else
    {
        for iter(ArrayList, it, list)
        {
            str_append(s, format_c(list->value_class, it.value));
            str_append(s, ", ");
        }
    }
    str_pop(s, 2); // Pop the last ', '
    str_append(s, "}");
    return s;
}

bool ArrayList__equals(ArrayList left, ArrayList right)
{
    if (!equals(Class, left->value_class, right->value_class) || left->length != right->length)
    {
        return false;
    }
    for (uint32_t i = 0; i < left->length; i++)
    {
        if (equals_c(left->value_class, left->values[i], right->values[i]))
        {
            return false;
        }
    }
    return true;
}

uint32_t ArrayList__hash(ArrayList instance)
{
    uint32_t h = 0;
    for iter(ArrayList, it, instance)
    {
        h = (31 * h + hash_c(instance->value_class, it.value));
    }
    return h;
}

int32_t ArrayList__compare(ArrayList left, ArrayList right)
{
    panic_if(!equals(Class, left->value_class, right->value_class), "compare(ArrayList<%s>, ArrayList<%s>) parameters have different generic types!", left->value_class->name, right->value_class->name);
    uint32_t min_length = min(left->length, right->length);
    for (uint32_t i = 0; i < min_length; i++)
    {
        int32_t result = compare_c(left->value_class, left->values[i], right->values[i]);
        if (result != 0)
        {
            return result;
        }
    }
    // Compare the lengths of each list
    return compare(uint32_t, left->length, right->length);
}

// Instance Methods

pointer_t al_pointer_t_get(ArrayList list, uint32_t index)
{
    panic_if(index >= list->length, "Index %d is outside of the valid range [0, %d)", index, list->length);
    return list->values[index];
}

void al_pointer_t_set(ArrayList list, uint32_t index, pointer_t value)
{
    panic_if(index >= list->length, "Index %d is outside of the valid range [0, %d)", index, list->length);
    pointer_t prev = list->values[index];
    if (prev != NULL)
    {
        del_c(list->value_class, prev); // Overwriting a previous value, so must delete it
    }
    list->values[index] = value;
}

void al_pointer_t_append(ArrayList list, pointer_t value)
{
    if (list->length == list->size)
    {
        // Resize the array
        uint32_t old_size = list->size;
        uint32_t new_size = old_size * 2;
        pointer_t* new_array = safe_malloc(sizeof(pointer_t) * new_size);

        memcpy(new_array, list->values, sizeof(pointer_t) * old_size);
        free(list->values);

        list->values = new_array;
        list->size = new_size;
    }
    // Append at index length
    list->values[list->length] = value;
    list->length++;
}

void al_pointer_t_clear(ArrayList list)
{
    for iter(ArrayList, it, list)
    {
        del_c(list->value_class, it.value);
    }
    list->length = 0;
}