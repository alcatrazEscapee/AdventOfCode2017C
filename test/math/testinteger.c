
TEST(test_int32_new, {
    int32_t* i32 = new(Int32, 5);
    ASSERT_EQUAL(*i32, 5, "Int32 != constructor value");
    del(Int32, i32);
});

TEST(test_int64_new, {
    int64_t* i64 = new(Int64, 5);
    ASSERT_EQUAL(*i64, 5, "Int64 != constructor value");
    del(Int64, i64);
})

TEST_GROUP(test_integer_main, {
    test_int32_new();
    test_int64_new();
});