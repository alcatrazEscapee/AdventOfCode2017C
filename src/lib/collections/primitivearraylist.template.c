// Template
// Implementation for PrimitiveArrayList(type)
// @param type : The type of the array list

// Local definitions
// Undef'd at the end of this template
#define PrimitiveArrayList_t CONCAT(ArrayList_, type)

// Class
impl_class(PrimitiveArrayList_t);

// Static Methods
PrimitiveArrayList_t CONCAT3(al_, type, _from_array)(type array[], uint32_t size)
{
    PrimitiveArrayList_t list = new(PrimitiveArrayList_t, size);
    for (uint32_t i = 0; i < size; i++)
    {
        list->values[i] = array[i];
    }
    list->length = size;
    return list;
}

// Class Methods
PrimitiveArrayList_t CONCAT3(ArrayList_, type, __new)(uint32_t initial_size)
{
    panic_if(initial_size < 1, "PrimitiveArrayList<" LITERAL(type) "> initial size must be positive, got %d", initial_size);

    PrimitiveArrayList_t list = class_malloc(PrimitiveArrayList_t);

    list->values = safe_malloc(PrimitiveArrayList_t, sizeof(type) * initial_size);
    list->size = initial_size;
    list->length = 0;

    return list;
}

void CONCAT3(ArrayList_, type, __del)(PrimitiveArrayList_t list)
{
    free(list->values);
    free(list);
}

PrimitiveArrayList_t CONCAT3(ArrayList_, type, __copy)(PrimitiveArrayList_t list)
{
    PrimitiveArrayList_t new_list = new(PrimitiveArrayList_t, list->length);
    for iter(PrimitiveArrayList_t, it, list)
    {
        new_list->values[it.index] = it.value;
    }
    new_list->length = list->length;
    return new_list;
}

String CONCAT3(ArrayList_, type, __format)(PrimitiveArrayList_t list)
{
    String s = new(String, "PrimitiveArrayList<" LITERAL(type) ">{");
    if (list->length == 0)
    {
        str_append_char(s, '}');
        return s;
    }
    else
    {
        for iter(PrimitiveArrayList_t, it, list)
        {
            str_append_string(s, format(type, it.value));
            str_append_slice(s, ", ");
        }
    }
    str_pop(s, 2); // Pop the last ', '
    str_append_char(s, '}');
    return s;
}

bool CONCAT3(ArrayList_, type, __equals)(PrimitiveArrayList_t left, PrimitiveArrayList_t right)
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

uint32_t CONCAT3(ArrayList_, type, __hash)(PrimitiveArrayList_t instance)
{
    uint32_t h = 0;
    for iter(PrimitiveArrayList_t, it, instance)
    {
        h = (31 * h + hash(type, it.value));
    }
    return h;
}

int32_t CONCAT3(ArrayList_, type, __compare)(PrimitiveArrayList_t left, PrimitiveArrayList_t right)
{
    uint32_t min_length = min(left->length, right->length);
    for (uint32_t i = 0; i < min_length; i++)
    {
        int32_t result = compare(type, left->values[i], right->values[i]);
        if (result != 0)
        {
            return result;
        }
    }
    // Compare the lengths of each list
    return compare(uint32_t, left->length, right->length);
}

// Instance Methods
type CONCAT3(al_, type, _get)(PrimitiveArrayList_t list, uint32_t index)
{
    panic_if(index >= list->length, "Index %d is outside of the valid range [0, %d)", index, list->length);
    return list->values[index];
}

void CONCAT3(al_, type, _set)(PrimitiveArrayList_t list, uint32_t index, type value)
{
    panic_if(index >= list->length, "Index %d is outside of the valid range [0, %d)", index, list->length);
    list->values[index] = value;
}

void CONCAT3(al_, type, _append)(PrimitiveArrayList_t list, type value)
{
    if (list->length == list->size)
    {
        // Resize the array
        uint32_t old_size = list->size;
        uint32_t new_size = old_size * 2;
        type* new_array = safe_malloc(PrimitiveArrayList_t, sizeof(type) * new_size);

        memcpy(new_array, list->values, sizeof(type) * old_size);
        free(list->values);

        list->values = new_array;
        list->size = new_size;
    }
    // Append at index length
    list->values[list->length] = value;
    list->length++;
}

void CONCAT3(al_, type, _clear)(PrimitiveArrayList_t list)
{
    // This is the simplest way to remove all values
    // Since anything outside the range [0, arl->length) is considered garbage data, this invalidates the entire thing as quickly and as efficiently as possible
    list->length = 0;
}

#undef type
#undef PrimitiveArrayList_t
