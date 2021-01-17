#include "../unittest.h"

// Tests for Result(T) with all primitive types

#define TYPE bool
#define SOME true
#include "testresult.template.c"

#define TYPE char
#define SOME 'A'
#include "testresult.template.c"

#define TYPE int32_t
#define SOME 123
#include "testresult.template.c"

#define TYPE uint32_t
#define SOME 123u
#include "testresult.template.c"

#define TYPE int64_t
#define SOME 123ul
#include "testresult.template.c"

#define TYPE uint64_t
#define SOME 123ul
#include "testresult.template.c"

// Tests for Result(pointer_t), used here as Result(String)

TEST(test_result_pointer_new, {
    Result(String) ok = Ok(String, new(String, "Hello World!"));
    Result(String) err = Err(String);

    String ok_value = unwrap(String, ok);
    ASSERT_TRUE(str_equals_content(ok_value, "Hello World!"), "Expected unwrapped value, got '%s'", ok_value->slice);

    del(String, ok_value);
});

TEST(test_result_pointer_is_ok_is_err, {
    Result(String) ok = Ok(String, new(String, "Hello World!"));
    Result(String) err = Err(String);

    ASSERT_TRUE(is_ok(ok), "Expected ok");
    ASSERT_TRUE(is_err(err), "Expected err");

    del(String, unwrap(String, ok));
});

TEST(test_result_pointer_unwrap_or, {
    Result(String) ok = Ok(String, new(String, "Hello World!"));
    Result(String) err = Err(String);

    String ok_value = unwrap_or(String, ok, new(String, "Defaulted!"));
    String err_value = unwrap_or(String, err, new(String, "Defaulted!"));

    ASSERT_TRUE(str_equals_content(ok_value, "Hello World!"), "Expected ok to unwrap -> got '%s'", ok_value->slice);
    ASSERT_TRUE(str_equals_content(err_value, "Defaulted!"), "Expected err to default -> got '%s'", err_value->slice);

    // Both should delete a value, the unused value (in the unwrap on ok) will be deleted by unwrap_or
    del(String, ok_value);
    del(String, err_value);
});

TEST_GROUP(test_result_pointer, {
    test_result_pointer_new();
    test_result_pointer_is_ok_is_err();
    test_result_pointer_unwrap_or();
});


TEST_GROUP(test_result, {
    test_result_bool();
    test_result_char();
    test_result_int32_t();
    test_result_uint32_t();
    test_result_int64_t();
    test_result_uint64_t();

    test_result_pointer();
});