// int32_t ArrayList

#include "intarraylist.h"

// Constructor
IntArrayList* IntArrayList__new(uint32_t initial_size)
{
    IntArrayList* p = (IntArrayList*) malloc(sizeof(IntArrayList));
    int32_t* values = (int32_t*) malloc(sizeof(int32_t) * initial_size);

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
        int32_t old_size = arl->size;
        int32_t new_size = old_size * 2;
        printf("Resizing an array from %d -> %d\n", old_size, new_size);
        int32_t* new_array = (int32_t*) malloc(sizeof(int32_t) * new_size);

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