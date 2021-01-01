
TEST(test_array_hash_map_new, {
    ArrayHashMap* map = new(ArrayHashMap, 10, class(Int32), class(Int32));
    del(ArrayHashMap, map);
});

TEST(test_array_hash_map_put_get, {
    ArrayHashMap* map = new(ArrayHashMap, 10, class(Int32), class(Int32));
    int32_t* key = new(Int32, 2);
    int32_t* bad_key = new(Int32, 5);

    ahm_put(map, copy(Int32, key), new(Int32, 4));

    ASSERT_TRUE(ahm_key_in(map, key), "Map should contain key = 2");
    ASSERT_FALSE(ahm_key_in(map, bad_key), "Map should not contain key = 5");

    int32_t* exp = new(Int32, 4);
    ASSERT_TRUE(equals(Int32, ahm_get(map, key), exp), "Expected to get val = 4 from key = 2");
    ASSERT_FALSE(equals(Int32, ahm_get(map, bad_key), exp), "Expected no value from key = 5");

    del(Int32, key);
    del(Int32, bad_key);
    del(Int32, exp);
    del(ArrayHashMap, map);
})

TEST(test_array_hash_map_put_replace, {
    ArrayHashMap* map = new(ArrayHashMap, 10, class(Int32), class(Int32));
    int32_t* key = new(Int32, 2);
    ahm_put(map, copy(Int32, key), new(Int32, 11));
    ahm_put(map, copy(Int32, key), new(Int32, 44));

    ASSERT_TRUE(ahm_key_in(map, key), "Map should contain key = 2");

    int32_t* exp = new(Int32, 44);
    ASSERT_TRUE(equals(Int32, ahm_get(map, key), exp), "Expected to get val = 44 from key = 2");

    del(Int32, key);
    del(Int32, exp);
    del(ArrayHashMap, map);
});

TEST(test_array_hash_map_format, {
    ArrayHashMap* map = new(ArrayHashMap, 10, class(Int32), class(Int32));

    String* s1 = format(ArrayHashMap, map);
    ASSERT_TRUE(str_equals_content(s1, "ArrayHashMap<Int32, Int32>{}"), "Actual: '%s'", s1->slice);
    del(String, s1);

    ahm_put(map, new(Int32, 0), new(Int32, 1234));
    ahm_put(map, new(Int32, 1), new(Int32, 4321));

    // Technically this is unordered, but since we know Int32's hash function for positive integers it itself, we can consider this a test
    String* s2 = format(ArrayHashMap, map);
    ASSERT_TRUE(str_equals_content(s2, "ArrayHashMap<Int32, Int32>{0: 1234, 1: 4321}"), "Actual: '%s'", s2->slice);
    del(String, s2);

    del(ArrayHashMap, map);
});

TEST_GROUP(test_array_hash_map_main, {
    test_array_hash_map_new();
    test_array_hash_map_put_get();
    test_array_hash_map_put_replace();
    test_array_hash_map_format();
});