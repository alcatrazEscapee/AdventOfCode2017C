// Unit Test Entry Point

#include "unittest.h"

void test_array_list();
void test_map();
void test_result();
void test_set();
void test_tuple();
void test_strings();
void test_utils();

uint32_t __failed_tests = 0;
uint32_t __passed_tests = 0;

int main(void)
{
    printf("-----\nTesting Starting\n-----\n\n");
    
    test_array_list();
    test_map();
    test_result();
    test_set();
    test_tuple();
    test_strings();
    test_utils();

    printf("\n-----\nTesting Complete\n-----\n\nPassed = %d\nFailed = %d\n", __passed_tests, __failed_tests);
    return 0;
}


void __test_invoke(slice_t test_name, FnTestInternal test_internal)
{
    bool passed = true;
    bool paniced = false;
    try
    {
        passed = test_internal();
    }
    catch
    {
        paniced = true;
    }
    finally;
    if (paniced)
    {
        printf(FORMAT_BOLD FORMAT_RED "Panicked: %s" FORMAT_RESET "\n", test_name);
        __failed_tests++;
    }
    else if (passed)
    {
        printf(FORMAT_GREEN "Passed: %s" FORMAT_RESET "\n", test_name);
        __passed_tests++;
    }
    else
    {
        printf(FORMAT_BOLD FORMAT_YELLOW "Failed: %s" FORMAT_RESET "\n", test_name);
        __failed_tests++;
    }
}