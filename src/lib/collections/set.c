#include "set.h"

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