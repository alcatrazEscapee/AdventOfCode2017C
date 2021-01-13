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
    
    list->values = safe_malloc(ArrayList, sizeof(pointer_t) * initial_size);
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
        str_append_char(s, '}');
        return s;
    }
    else
    {
        for iter(ArrayList, it, list)
        {
            str_append_string(s, format_c(list->value_class, it.value));
            str_append_slice(s, ", ");
        }
    }
    str_pop(s, 2); // Pop the last ', '
    str_append_char(s, '}');
    return s;
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
        pointer_t* new_array = safe_malloc(ArrayList, sizeof(pointer_t) * new_size);

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