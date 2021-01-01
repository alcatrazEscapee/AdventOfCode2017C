// int32_t ArrayList

#include "intarraylist.h"

// Static Methods

IntArrayList* ial_create_from_array(int32_t* array, uint32_t size)
{
    IntArrayList* list = new(IntArrayList, size);
    for (uint32_t i = 0; i < size; i++)
    {
        list->values[i] = array[i];
    }
    list->length = size;
    return list;
}

// Class
DERIVE_CLASS(IntArrayList);

// Class Methods
IntArrayList* IntArrayList__new(uint32_t initial_size)
{
    PANIC_IF(initial_size < 1, "IntArrayList initial size must be positive, got %d", initial_size);

    IntArrayList* p = (IntArrayList*) malloc(sizeof(IntArrayList));
    int32_t* values = (int32_t*) malloc(sizeof(int32_t) * initial_size);

    PANIC_IF_NULL(p, "Unable to create IntArrayList with initial_size %d", initial_size);
    PANIC_IF_NULL(values, "Unable to create IntArrayList with initial_size %d", initial_size);

    p->values = values;
    p->size = initial_size;
    p->length = 0;

    return p;
}

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
        new_list->values[it.index] = it.value;
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
            str_append_string(s, str_format("%d", it.value));
            str_append_slice(s, ", ");
        }
    }
    str_pop(s, 2); // Pop the last ', '
    str_append_char(s, '}');
    return s;
}

bool IntArrayList__equals(IntArrayList* left, IntArrayList* right)
{
    if (left->length != right->length)
    {
        return false;
    }
    for (uint32_t i = 0; i < left->length; i++)
    {
        if (left->values[i] != right->values[i])
        {
            return false;
        }
    }
    return true;
}

uint32_t IntArrayList__hash(IntArrayList* instance)
{
    uint32_t h = 1;
    for iter(IntArrayList, it, instance)
    {
        h = (31 * h + (uint32_t) it.value);
    }
    return h;
}

int32_t IntArrayList__compare(IntArrayList* left, IntArrayList* right)
{
    uint32_t min_length = min(left->length, right->length);
    for (uint32_t i = 0; i < min_length; i++)
    {
        int32_t left_val = left->values[i];
        int32_t right_val = right->values[i];
        if (left_val < right_val)
        {
            return -1;
        }
        else if (left_val > right_val)
        {
            return 1;
        }
    }
    if (left->length < right->length)
    {
        return -1;
    }
    else if (left->length > right->length)
    {
        return 1;
    }
    return 0;
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