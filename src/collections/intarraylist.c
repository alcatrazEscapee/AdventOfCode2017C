// int32_t ArrayList

#include "intarraylist.h"

// Constructor
IntArrayList* IntArrayList__new(uint32_t initial_size)
{
    IntArrayList* p = (IntArrayList*) malloc(sizeof(IntArrayList));
    int32_t* values = (int32_t*) malloc(sizeof(int32_t) * initial_size);

    PANIC_IF_NULL(p, "Unable to create IntArrayList with initial_size %d", initial_size);
    PANIC_IF_NULL(values, "Unable to create IntArrayList with initial_size %d", initial_size);

    p->values = values;
    p->size = initial_size;
    p->length = 0;

    return p;
}

// Destructor
void IntArrayList__del(IntArrayList* arl)
{
    free(arl->values);
    free(arl);
}

IntArrayList* IntArrayList__copy(IntArrayList* list)
{
    IntArrayList* new_list = new(IntArrayList, list->length);
    for iter(IntArrayList, it, list)
    {
        new_list->values[it->index] = it->value;
    }
    new_list->length = list->length;
    return new_list;
}

String* IntArrayList__format(IntArrayList* list)
{
    String* s = new(String, "IntArrayList{");
    if (list->length == 0)
    {
        str_append_char(s, '}');
        return s;
    }
    else
    {
        for iter(IntArrayList, it, list)
        {
            str_append_string(s, str_format("%d", it->value));
            str_append_slice(s, ", ");
        }
    }
    str_pop(s, 2); // Pop the last ', '
    str_append_char(s, '}');
    return s;
}


// Iterator

Iterator(IntArrayList)* IntArrayList__iterator__new(IntArrayList* list)
{
    Iterator(IntArrayList)* it = malloc(sizeof(Iterator(IntArrayList)));
    PANIC_IF_NULL(it, "Unable to create Iterator<IntArrayList>");

    it->index = 0;
    it->value = 0;

    return it;
}

void IntArrayList__iterator__del(Iterator(IntArrayList)* it)
{
    free(it);
}


// Instance Methods

bool ial_in(IntArrayList* arl, uint32_t index)
{
    return index < arl->length;
}

int32_t ial_get(IntArrayList* arl, uint32_t index)
{
    if (!ial_in(arl, index)) PANIC("Index %d is outside of the valid range [0, %d) at ial_get(IntArrayList*, int32_t)\n", index, arl->length);
    return arl->values[index];
}

void ial_set(IntArrayList* arl, uint32_t index, int32_t value)
{
    if (!ial_in(arl, index)) PANIC("Index %d is outside of the valid range [0, %d) at ial_set(IntArrayList*, int32_t)\n", index, arl->length);
    arl->values[index] = value;
}

void ial_append(IntArrayList* arl, int32_t value)
{
    if (arl->length == arl->size)
    {
        // Resize the array
        uint32_t old_size = arl->size;
        uint32_t new_size = old_size * 2;
        int32_t* new_array = (int32_t*) malloc(sizeof(int32_t) * new_size);
        PANIC_IF_NULL(new_array, "Unable to resize IntArrayList from %d to %d", old_size, new_size);

        memcpy(new_array, arl->values, sizeof(int32_t) * old_size);
        free(arl->values);

        arl->values = new_array;
        arl->size = new_size;
    }
    // Append at index length
    arl->values[arl->length] = value;
    arl->length++;
}

void ial_clear(IntArrayList* arl)
{
    // This is the simplest way to remove all values
    // Since anything outside the range [0, arl->length) is considered garbage data, this invalidates the entire thing as quickly and as efficiently as possible
    arl->length = 0;
}