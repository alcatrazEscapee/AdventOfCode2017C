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
    body \
    ; \
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
    jmp_buf __fail_ctx; \
    if (!setjmp(__fail_ctx)) \
    { \
        try \
        { \
            body \
            ; \
        } \
        catch \
        { \
            __paniced = true; \
        } \
        finally; \
    } \
    else \
    { \
        __passed = false; \
    } \
    if (__paniced) \
    { \
        printf(FORMAT_BOLD FORMAT_RED "Panicked: " LITERAL(name) FORMAT_RESET "\n"); \
        __failed_tests++; \
    } \
    else if (__passed) \
    { \
        printf(FORMAT_GREEN "Passed: " LITERAL(name) FORMAT_RESET "\n"); \
        __passed_tests++; \
    } \
    else \
    { \
        printf(FORMAT_BOLD FORMAT_YELLOW "Failed: " LITERAL(name) FORMAT_RESET "\n"); \
        __failed_tests++; \
    } \
}

#define FAIL(args...) \
do { \
    printf(args); \
    longjmp(__fail_ctx, 1); \
} while (0)

#define ASSERT_TRUE(condition, format_string, args...) \
do { \
    if (!(condition)) \
    { \
        printf("Assert True Failed:\n\t" LITERAL(condition) "\n\t"); \
        printf(format_string "\n", ## args); \
        longjmp(__fail_ctx, 1); \
    } \
} while(0)

#define ASSERT_FALSE(condition, format_string, args...) \
do { \
    if (condition) \
    { \
        printf("Assert False Failed:\n\t" LITERAL(condition) "\n\t"); \
        printf(format_string "\n", ## args); \
        longjmp(__fail_ctx, 1); \
    } \
} while (0)

#define ASSERT_EQUAL(left, right, format_string, args...) \
do { \
    if ((left) != (right)) \
    { \
        printf("Assert Equal Failed.\n\t'" LITERAL(left) "' != '" LITERAL(right) "'\n\t"); \
        printf(format_string "\n", ## args); \
        longjmp(__fail_ctx, 1); \
    } \
} while (0)

#define ASSERT_NOT_EQUAL(left, right, format_string, args...) \
do { \
    if ((left) == (right)) \
    { \
        printf("Assert Not Equal Failed.\n\t'" LITERAL(left) "' != '" LITERAL(right) "'\n\t"); \
        printf(format_string "\n", ## args); \
        longjmp(__fail_ctx, 1); \
    } \
} while (0)

#endif

