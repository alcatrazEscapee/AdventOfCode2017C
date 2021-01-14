#include "set.h"

Set Set__new(uint32_t initial_size, Class key_class)
{
    return new(Map, initial_size, key_class, class(Void));
}

void Set__del(Set set)
{
    return del(Map, set);
}

String Set__format(Set set)
{
    String s = new(String, "Set<");
    str_append_slice(s, set->key_class->name);
    str_append_slice(s, ">{");
    if (set->length == 0)
    {
        str_append_char(s, '}');
        return s;
    }
    else
    {
        for iter(Set, it, set)
        {
            str_append_string(s, format_c(set->key_class, it.key));
            str_append_slice(s, ", ");
        }
    }
    str_pop(s, 2); // Pop the last ', '
    str_append_char(s, '}');
    return s;
}