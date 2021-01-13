#include "../unittest.h"

// Indvidual tests for each primitive array list type using templates

#define type char
#define v1 'A'
#define v1s "A"
#include "testprimitivearraylist.template.c"

#define type bool
#define v1 true
#define v1s "true"
#include "testprimitivearraylist.template.c"

#define type int32_t
#define v1 123
#include "testprimitivearraylist.template.c"

#define type int64_t
#define v1 123
#include "testprimitivearraylist.template.c"

#define type uint32_t
#define v1 123
#include "testprimitivearraylist.template.c"

#define type uint64_t
#define v1 123
#include "testprimitivearraylist.template.c"

TEST_GROUP(test_array_list, {

    // Primitive ArrayList groups
    test_char_list();
    test_bool_list();
    test_int32_t_list();
    test_int64_t_list();
    test_uint32_t_list();
    test_uint64_t_list();
})