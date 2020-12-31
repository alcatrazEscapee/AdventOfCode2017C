// Generic ArrayList

#include "arraylist.h"

// Constructor
ArrayList* ArrayList__new(uint32_t initial_size, Class* value_class)
{
    ArrayList* p = (ArrayList*) malloc(sizeof(ArrayList));
    void** values = (void**) malloc(sizeof(void*) * initial_size);

    PANIC_IF_NULL(p, "Unable to create ArrayList with initial_size %d", initial_size);
    PANIC_IF_NULL(values, "Unable to create ArrayList with initial_size %d", initial_size);

    p->values = values;
    p->value_class = value_class;
    p->size = initial_size;
    p->length = 0;

    return p;
}

// Destructor
void ArrayList__del(ArrayList* arl)
{
    al_clear(arl);
    free(arl->values);
    free(arl);
}

String* ArrayList__format(ArrayList* list)
{
    String* s = new(String, "ArrayList{");
    if (list->length == 0)
    {
        str_append_char(s, '}');
        return s;
    }
    else
    {
        for iter(ArrayList, it, list)
        {
            str_append_string(s, format_c(list->value_class, it->value));
            str_append_slice(s, ", ");
        }
    }
    str_pop(s, 2); // Pop the last ', '
    str_append_char(s, '}');
    return s;
}


// Iterator

Iterator(ArrayList)* ArrayList__iterator__new(ArrayList* list)
{
    Iterator(ArrayList)* it = malloc(sizeof(Iterator(ArrayList)));
    PANIC_IF_NULL(it, "Unable to create Iterator<ArrayList<%s>>", list->value_class->name);

    it->index = 0;
    it->value = 0;

    return it;
}

void ArrayList__iterator__del(Iterator(ArrayList)* it)
{
    free(it);
}


// Instance Methods

bool al_in(ArrayList* arl, uint32_t index)
{
    return index < arl->length;
}

void* al_get(ArrayList* arl, uint32_t index)
{
    if (!al_in(arl, index)) PANIC("Index %d is outside of the valid range [0, %d) at al_get(ArrayList*, int32_t)\n", index, arl->length);
    return arl->values[index];
}

void al_set(ArrayList* arl, uint32_t index, void* value)
{
    if (!al_in(arl, index)) PANIC("Index %d is outside of the valid range [0, %d) at al_set(ArrayList*, int32_t)\n", index, arl->length);
    arl->values[index] = value;
}

void al_append(ArrayList* arl, void* value)
{
    if (arl->length == arl->size)
    {
        // Resize the array
        uint32_t old_size = arl->size;
        uint32_t new_size = old_size * 2;
        void** new_array = (void**) malloc(sizeof(void*) * new_size);
        PANIC_IF_NULL(new_array, "Unable to resize ArrayList from %d to %d", old_size, new_size);

        memcpy(new_array, arl->values, sizeof(void*) * old_size);
        free(arl->values);

        arl->values = new_array;
        arl->size = new_size;
    }
    // Append at index length
    arl->values[arl->length] = value;
    arl->length++;
}

void al_clear(ArrayList* list)
{
    for iter(ArrayList, it, list)
    {
        del_c(list->value_class, it->value);
    }
    list->length = 0;
}