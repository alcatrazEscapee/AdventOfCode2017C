// Unit Test Entry Point

#define VERBOSE_ON_PASS

#include "unittest.h"
#include "../src/common.h"

#include "collections/testarrayhashmap.c"
#include "collections/testarraylist.c"
#include "collections/testintarraylist.c"

#include "math/testinteger.c"
#include "teststrings.c"

TEST_MAIN({

    test_array_hash_map_main();
    test_array_list_main();
    test_int_array_list_main();

    test_integer_main();
    test_strings_main();
});