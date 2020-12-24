// Unit Test Entry Point

#include "unittest.h"

#include "math/testinteger.c"
#include "teststrings.c"

TEST_MAIN({

    test_integer_main();
    test_strings_main();
});