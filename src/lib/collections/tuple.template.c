// If this file is included directly, the header should not undef the arguments at the end
#define TUPLE_TEMPLATE_SKIP_UNDEFS
#include "tuple.template.h"

// Class
impl_class(TUPLE_CLASS);

// Class Implementation
// Constructor
TUPLE_CLASS CONCAT(TUPLE_CLASS, __new)( IIF_LIST(TUPLE_NARGS)(TUPLE_TYPE1 TUPLE_VALUE1, TUPLE_TYPE2 TUPLE_VALUE2, TUPLE_TYPE3 TUPLE_VALUE3, TUPLE_TYPE4 TUPLE_VALUE4) )
{
    TUPLE_CLASS tuple = class_malloc(TUPLE_CLASS);
    tuple->TUPLE_VALUE1 = TUPLE_VALUE1;
    IIF_EMPTY(TUPLE_ARG2, tuple->TUPLE_VALUE2 = TUPLE_VALUE2;)
    IIF_EMPTY(TUPLE_ARG3, tuple->TUPLE_VALUE3 = TUPLE_VALUE3;)
    IIF_EMPTY(TUPLE_ARG4, tuple->TUPLE_VALUE4 = TUPLE_VALUE4;)
    return tuple;
}

// Destructor
void CONCAT(TUPLE_CLASS, __del)(TUPLE_CLASS tuple)
{
    del(TUPLE_TYPE1, tuple->TUPLE_VALUE1);
    IIF_EMPTY(TUPLE_ARG2, del(TUPLE_TYPE2, tuple->TUPLE_VALUE2);)
    IIF_EMPTY(TUPLE_ARG3, del(TUPLE_TYPE3, tuple->TUPLE_VALUE3);)
    IIF_EMPTY(TUPLE_ARG4, del(TUPLE_TYPE4, tuple->TUPLE_VALUE4);)
    free(tuple);
}

// copy()
TUPLE_CLASS CONCAT(TUPLE_CLASS, __copy)(TUPLE_CLASS tuple)
{
    return new(TUPLE_CLASS,  IIF_LIST(TUPLE_NARGS)(copy(TUPLE_TYPE1, tuple->TUPLE_VALUE1), copy(TUPLE_TYPE2, tuple->TUPLE_VALUE2), copy(TUPLE_TYPE3, tuple->TUPLE_VALUE3), copy(TUPLE_TYPE4, tuple->TUPLE_VALUE4)) );
}

// equals()
bool CONCAT(TUPLE_CLASS, __equals)(TUPLE_CLASS left, TUPLE_CLASS right)
{
    return equals(TUPLE_TYPE1, left->TUPLE_VALUE1, right->TUPLE_VALUE1)
        IIF_EMPTY(TUPLE_ARG2, && equals(TUPLE_TYPE2, left->TUPLE_VALUE2, right->TUPLE_VALUE2))
        IIF_EMPTY(TUPLE_ARG3, && equals(TUPLE_TYPE3, left->TUPLE_VALUE3, right->TUPLE_VALUE3))
        IIF_EMPTY(TUPLE_ARG4, && equals(TUPLE_TYPE4, left->TUPLE_VALUE4, right->TUPLE_VALUE4))
    ;
}

// compare()
int32_t CONCAT(TUPLE_CLASS, __compare)(TUPLE_CLASS left, TUPLE_CLASS right)
{
    int32_t result = compare(TUPLE_TYPE1, left->TUPLE_VALUE1, right->TUPLE_VALUE1);
    if (result != 0) return result;
    IIF_EMPTY(TUPLE_ARG2, result = compare(TUPLE_TYPE2, left->TUPLE_VALUE2, right->TUPLE_VALUE2); if (result != 0) return result;)
    IIF_EMPTY(TUPLE_ARG3, result = compare(TUPLE_TYPE3, left->TUPLE_VALUE3, right->TUPLE_VALUE3); if (result != 0) return result;)
    IIF_EMPTY(TUPLE_ARG4, result = compare(TUPLE_TYPE4, left->TUPLE_VALUE4, right->TUPLE_VALUE4); if (result != 0) return result;)
    return result;
}

// hash()
uint32_t CONCAT(TUPLE_CLASS, __hash)(TUPLE_CLASS tuple)
{
    uint32_t h = hash(TUPLE_TYPE1, tuple->TUPLE_VALUE1);
    IIF_EMPTY(TUPLE_ARG2, h ^= hash(TUPLE_TYPE2, tuple->TUPLE_VALUE2);)
    IIF_EMPTY(TUPLE_ARG3, h ^= hash(TUPLE_TYPE3, tuple->TUPLE_VALUE3);)
    IIF_EMPTY(TUPLE_ARG4, h ^= hash(TUPLE_TYPE4, tuple->TUPLE_VALUE4);)
    return h;
}

// format()
// Result the Tuple Name, with types in generics
String CONCAT(TUPLE_CLASS, __format)(TUPLE_CLASS tuple)
{
    String string = new(String, LITERAL(TUPLE_CLASS) "<" LITERAL(IIF_LIST(TUPLE_NARGS)(TUPLE_TYPE1, TUPLE_TYPE2, TUPLE_TYPE3, TUPLE_TYPE4)) ">(");
    str_append_string(string, format(TUPLE_TYPE1, tuple->TUPLE_VALUE1));
    str_append_slice(string, ", ");
    IIF_EMPTY(TUPLE_ARG2, str_append_string(string, format(TUPLE_TYPE2, tuple->TUPLE_VALUE2)); str_append_slice(string, ", ");)
    IIF_EMPTY(TUPLE_ARG3, str_append_string(string, format(TUPLE_TYPE3, tuple->TUPLE_VALUE3)); str_append_slice(string, ", ");)
    IIF_EMPTY(TUPLE_ARG4, str_append_string(string, format(TUPLE_TYPE4, tuple->TUPLE_VALUE4)); str_append_slice(string, ", ");)
    str_pop(string, 2);
    str_append_char(string, ')');
    return string;
}

// Clear local definitions
#undef TUPLE_TEMPLATE_SKIP_UNDEFS
#undef Tuple