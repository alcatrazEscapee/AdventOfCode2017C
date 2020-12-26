// Simple, easy-to-write unit tests
//
// How to use:
//
// 1. Create a entry point file (say unittest.c)
// 2. Include this header
// 3. Include (directly) any and all other C source files (i.e. #include <other_source_file.c>).
// 4. Write tests. A test consists of the TEST macro:
// TEST(the_test_name, { ... })
// Put anything inside the code block you wish to run on the test, including assertions:
// An assertion consists of an ASSERT_[FOO] macro. (ASSERT_TRUE, ASSERT_FALSE, ASSERT_EQUAL, etc.)
// The assertion takes arguments to compare against, and a message (which must be a format string, followed by optional format arguments) to print on failure
// A sample test may look like:
// TEST(testIntegerEquality, ASSERT_EQUAL(3, 3, "Three is not equal to itself?"));
// 5. At the bottom if the file, include the TEST_MAIN({ ... }) macro, and inside the code block invoke (as a function) every test name.
// e.g. TEST_MAIN({ testIntegerEquality(); });
// Then compile unittest.c, and run it!
//
// Optional Features:
//
// Including a #define VERBOSE_ON_PASS will output additional output on the result of every passed test, if desired

#ifndef UNITTEST_H
#define UNITTEST_H

#include <stdio.h> // printf, etc.
#include <stdint.h> // int32_t, etc.
#include <stdbool.h> // true, false, bool

extern uint32_t __failed_tests;
extern uint32_t __passed_tests;
extern bool __verbose_on_pass;

#ifdef VERBOSE_ON_PASS
#define __verbose_on_pass true
#else
#define __verbose_on_pass false
#endif 

#define TEST_MAIN(body...) \
uint32_t __passed_tests = 0; \
uint32_t __failed_tests = 0; \
int main(void) \
{ \
    printf("-----\nTesting Starting\n-----\n\n"); \
    body \
    ; \
    printf("\n-----\nTesting Complete\n-----\n\nPassed = %d\nFailed = %d\n", __passed_tests, __failed_tests); \
    return 0; \
}

#define TEST_GROUP(name, body...) \
void name(); \
void name() \
{ \
    body \
    ; \
}


#define TEST(name, body...) \
void name(); \
void name() \
{ \
    bool __passed = true; \
    body \
    ; \
    if (__passed) \
    { \
        if (__verbose_on_pass) printf("Passed: %s\n", #name); \
        __passed_tests++; \
    } \
    else \
    { \
        printf("Failed: %s\n\n", #name); \
        __failed_tests++; \
    } \
}

#define FAIL(args...) \
do { \
    printf(args); \
    __passed = false; \
} while (0)

#define ASSERT_TRUE(condition, args...) \
do { \
    if (!(condition)) \
    { \
        printf("Assert True Failed:\n\t%s\n\t", #condition); \
        FAIL(args); \
        printf("\n"); \
    } \
} while(0)

#define ASSERT_FALSE(condition, args...) \
do { \
    if (condition) \
    { \
        printf("Assert False Failed:\n\t%s\n\t", #condition); \
        FAIL(args); \
        printf("\n"); \
    } \
} while (0)

#define ASSERT_EQUAL(left, right, args...) \
do { \
    if ((left) != (right)) \
    { \
        printf("Assert Equal Failed.\n\t'%s' != '%s'\n\t", #left, #right); \
        FAIL(args); \
        printf("\n"); \
    } \
} while (0)

#endif

