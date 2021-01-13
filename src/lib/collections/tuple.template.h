// Template
// Header for Tuple(...)
// @param Tuple - the arguments to the tuple construction. See below for usage.
//
// Usage:
// #define Tuple TupleClassName, type1, value1, type2, value2, ... typeN, valueN
// #include "tuple.template.h"

struct CONCAT(TUPLE_CLASS, __struct)
{
    TUPLE_TYPE1 TUPLE_VALUE1;
    IIF_EMPTY(TUPLE_ARG2, TUPLE_TYPE2 TUPLE_VALUE2;)
    IIF_EMPTY(TUPLE_ARG3, TUPLE_TYPE3 TUPLE_VALUE3;)
    IIF_EMPTY(TUPLE_ARG4, TUPLE_TYPE4 TUPLE_VALUE4;)
};

typedef struct CONCAT(TUPLE_CLASS, __struct) * TUPLE_CLASS;

declare_class(TUPLE_CLASS);
declare_constructor(TUPLE_CLASS, IIF_LIST(TUPLE_NARGS)(TUPLE_TYPE1 TUPLE_VALUE1, TUPLE_TYPE2 TUPLE_VALUE2, TUPLE_TYPE3 TUPLE_VALUE3, TUPLE_TYPE4 TUPLE_VALUE4));

// Clear local definitions
#ifndef TUPLE_TEMPLATE_SKIP_UNDEFS
#undef Tuple
#endif