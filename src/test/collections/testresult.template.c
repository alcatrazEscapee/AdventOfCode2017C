// Template for Result(T) tests
// @param TYPE: The type param for the result T
// @param SOME: A non-default value for T

#define VALUE default_value(TYPE)

TEST(CONCAT3(test_result_, TYPE, _new), {
    Result(TYPE) ok = Ok(TYPE, VALUE);
    Result(TYPE) err = Err(TYPE);

    TYPE ok_value = unwrap(ok);
    ASSERT_EQUAL(ok_value, VALUE, "Expected unwrapped value");
});

TEST(CONCAT3(test_result_, TYPE, _is_ok_is_err), {
    Result(TYPE) ok = Ok(TYPE, VALUE);
    Result(TYPE) err = Err(TYPE);

    ASSERT_TRUE(is_ok(ok), "Expected ok");
    ASSERT_TRUE(is_err(err), "Expected err");
});

TEST(CONCAT3(test_result_, TYPE, _unwrap_or), {
    Result(TYPE) ok = Ok(TYPE, VALUE);
    Result(TYPE) err = Err(TYPE);

    TYPE ok_value = unwrap_or(ok, SOME);
    TYPE err_value = unwrap_or(err, SOME);

    ASSERT_EQUAL(ok_value, VALUE, "Expected ok to unwrap");
    ASSERT_EQUAL(err_value, SOME, "Expected err to default");
});

TEST_GROUP(CONCAT(test_result_, TYPE), {
    CONCAT3(test_result_, TYPE, _new)();
    CONCAT3(test_result_, TYPE, _is_ok_is_err)();
    CONCAT3(test_result_, TYPE, _unwrap_or)();
});

#undef TYPE
#undef VALUE
#undef SOME