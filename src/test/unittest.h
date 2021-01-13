// Simple, easy-to-write unit tests

#include "../lib/lib.h"

#ifndef UNITTEST_H
#define UNITTEST_H

extern uint32_t __failed_tests;
extern uint32_t __passed_tests;

#define TEST_INCLUDE(name) \
void name(); \

#define TEST_GROUP(name, body...) \
void name(); \
void name() \
{ \
    printf("Group: " LITERAL(name) "\n"); \
    body \
    ; \
    printf("\n"); \
}

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

#define TEST(name, body...) \
static void name(); \
static void name() \
{ \
    bool __passed = true; \
    bool __paniced = false; \
    try \
    { \
    body \
    ; \
    } \
    catch \
    { \
        String __ex = exception; \
        printf("\tPaniced: " LITERAL(name) "\nCaught Exception:\n%s", __ex->slice); \
        __paniced = true; \
        __failed_tests++; \
        del(String, __ex); \
    } \
    finally; \
    if (!__paniced) \
    { \
        if (__passed) \
        { \
            printf("\tPassed: " LITERAL(name) "\n"); \
            __passed_tests++; \
        } \
        else \
        { \
            printf("\tFailed: " LITERAL(name) "\n"); \
            __failed_tests++; \
        } \
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
        printf("Assert True Failed:\n\t" LITERAL(condition) "\n\t"); \
        FAIL(args); \
        printf("\n"); \
    } \
} while(0)

#define ASSERT_FALSE(condition, args...) \
do { \
    if (condition) \
    { \
        printf("Assert False Failed:\n\t" LITERAL(condition) "\n\t"); \
        FAIL(args); \
        printf("\n"); \
    } \
} while (0)

#define ASSERT_EQUAL(left, right, args...) \
do { \
    if ((left) != (right)) \
    { \
        printf("Assert Equal Failed.\n\t'" LITERAL(left) "' != '" LITERAL(right) "'\n\t"); \
        FAIL(args); \
        printf("\n"); \
    } \
} while (0)

#define ASSERT_NOT_EQUAL(left, right, args...) \
do { \
    if ((left) == (right)) \
    { \
        printf("Assert Not Equal Failed.\n\t'" LITERAL(left) "' != '" LITERAL(right) "'\n\t"); \
        FAIL(args); \
        printf("\n"); \
    } \
} while (0)

#endif

