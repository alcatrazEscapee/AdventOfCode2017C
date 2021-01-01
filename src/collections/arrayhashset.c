#include "arrayhashset.h"

ArrayHashSet* ArrayHashSet__new(uint32_t initial_size, Class* key_class)
{
    return (ArrayHashSet*) new(ArrayHashMap, initial_size, key_class, class(Void));
}

void ArrayHashSet__del(ArrayHashSet* set)
{
    del(ArrayHashMap, set);
}

String* ArrayHashSet__format(ArrayHashSet* set)
{
    String* s = new(String, "ArrayHashSet<");
    str_append_slice(s, set->key_class->name);
    str_append_slice(s, ">{");
    if (set->length == 0)
    {
        str_append_char(s, '}');
        return s;
    }
    else
    {
        for iter(ArrayHashSet, it, set)
        {
            str_append_string(s, format_c(set->key_class, it.key));
            str_append_slice(s, ", ");
        }
    }
    str_pop(s, 2); // Pop the last ', '
    str_append_char(s, '}');
    return s;
}

// Public Instance Methods - these all borrow the set

bool ahs_put(ArrayHashSet* set, void* key)
{
    return ahm_put(set, key, NULL);
}

bool ahs_in(ArrayHashSet* set, void* key)
{
    return ahm_key_in(set, key);
}

void ahs_clear(ArrayHashSet* set)
{
    ahm_clear(set);
}