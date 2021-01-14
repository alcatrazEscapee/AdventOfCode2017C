// Unit Test Entry Point

#include "unittest.h"

TEST_INCLUDE(test_array_list);
TEST_INCLUDE(test_map);
TEST_INCLUDE(test_tuple);
TEST_INCLUDE(test_strings);
TEST_INCLUDE(test_utils);


TEST_MAIN({
    test_array_list();
    test_map();
    test_tuple();
    test_strings();
    test_utils();
});