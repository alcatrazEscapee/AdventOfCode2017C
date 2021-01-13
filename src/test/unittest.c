// Unit Test Entry Point

#include "unittest.h"

TEST_INCLUDE(test_array_list);
TEST_INCLUDE(test_strings);
TEST_INCLUDE(test_map);

TEST_MAIN({
    test_array_list();
    test_map();
    test_strings();
});