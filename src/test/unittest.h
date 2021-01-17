// Simple, easy-to-write unit tests

#include "../lib/lib.h"

#ifndef UNITTEST_H
#define UNITTEST_H

// Test Function
// Return value indicates passing
typedef bool (*FnTestInternal) ();

// Test Invocation function
// Test methods are passed to this, then ran and output recorded
void __test_invoke(slice_t test_name, FnTestInternal test_internal);

// Defines a group of test methods.
// Useful for template-based grouping of similar tests
#define TEST_GROUP(name, body...) \
void name(); \
void name() \
{ \
    body \
    ; \
} \
GLOBAL_NOOP

// Defines a single test method, private to the file it is defined in
// To call outside the file, call from a TEST_GROUP() defined in the same file
#define TEST(name, body...) \
static void name(); \
static bool CONCAT(__internal_, name) (); \
\
static void name() { __test_invoke(LITERAL(name), & CONCAT(__internal_, name)); } \
static bool CONCAT(__internal_, name) () \
{ \
    body \
    ; \
    return true; \
} \
GLOBAL_NOOP

// Fails a test unconditionally
#define FAIL(args...) \
do { \
    printf(args); \
    return false; \
} while (0)

// Various assertions
// format_string and args... are as to printf()
// As a general rule, the detail should be the actual result, not the expected one. e.g. ASSERT_EQUAL(x, 3, "Actual: %d", x);
// The expected result is already able to be inferred through stringification.
#define ASSERT_TRUE(condition, format_string, args...) \
do { \
    if (!(condition)) \
    { \
        printf("Assert True Failed:\n\t" LITERAL(condition) "\n\t"); \
        printf(format_string "\n", ## args); \
        return false; \
    } \
} while(0)

#define ASSERT_FALSE(condition, format_string, args...) \
do { \
    if (condition) \
    { \
        printf("Assert False Failed:\n\t" LITERAL(condition) "\n\t"); \
        printf(format_string "\n", ## args); \
        return false; \
    } \
} while (0)

#define ASSERT_EQUAL(left, right, format_string, args...) \
do { \
    if ((left) != (right)) \
    { \
        printf("Assert Equal Failed.\n\t'" LITERAL(left) "' != '" LITERAL(right) "'\n\t"); \
        printf(format_string "\n", ## args); \
        return false; \
    } \
} while (0)

#define ASSERT_NOT_EQUAL(left, right, format_string, args...) \
do { \
    if ((left) == (right)) \
    { \
        printf("Assert Not Equal Failed.\n\t'" LITERAL(left) "' != '" LITERAL(right) "'\n\t"); \
        printf(format_string "\n", ## args); \
        return false; \
    } \
} while (0)

#endif

