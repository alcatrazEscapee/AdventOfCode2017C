// Unit Test Entry Point

#define VERBOSE_ON_PASS

#include "unittest.h"

#include "collections/testarrayhashmap.c"
#include "math/testinteger.c"
#include "teststrings.c"

TEST_MAIN({

    test_array_hash_map_main();

    test_integer_main();
    test_strings_main();
});