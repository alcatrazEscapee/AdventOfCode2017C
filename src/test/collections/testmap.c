
#include "../unittest.h"

TEST(test_map_new, {
    Map map = new(Map, 10, class(Int32), class(Int32));
    del(Map, map);
});

TEST(test_map_put_get, {
    Map map = new(Map, 10, class(Int32), class(Int32));
    Int32 key = new(Int32, 2);
    Int32 bad_key = new(Int32, 5);

    map_put(map, copy(Int32, key), new(Int32, 4));

    ASSERT_TRUE(map_contains_key(map, key), "Map should contain key = 2");
    ASSERT_FALSE(map_contains_key(map, bad_key), "Map should not contain key = 5");

    Int32 exp = new(Int32, 4);
    ASSERT_TRUE(equals(Int32, map_get(map, key), exp), "Expected to get val = 4 from key = 2");
    ASSERT_FALSE(equals(Int32, map_get(map, bad_key), exp), "Expected no value from key = 5");

    del(Int32, key);
    del(Int32, bad_key);
    del(Int32, exp);
    del(Map, map);
});

TEST(test_map_put_replace, {
    Map map = new(Map, 10, class(Int32), class(Int32));
    Int32 key = new(Int32, 2);
    map_put(map, copy(Int32, key), new(Int32, 11));
    map_put(map, copy(Int32, key), new(Int32, 44));

    ASSERT_TRUE(map_contains_key(map, key), "Map should contain key = 2");

    Int32 exp = new(Int32, 44);
    ASSERT_TRUE(equals(Int32, map_get(map, key), exp), "Expected to get val = 44 from key = 2");

    del(Int32, key);
    del(Int32, exp);
    del(Map, map);
});

TEST(test_map_format, {
    Map map = new(Map, 10, class(Int32), class(Int32));

    String s1 = format(Map, map);
    ASSERT_TRUE(str_equals_content(s1, "Map<Int32, Int32>{}"), "Actual: '%s'", s1->slice);
    del(String, s1);

    map_put(map, new(Int32, 0), new(Int32, 1234));
    map_put(map, new(Int32, 1), new(Int32, 4321));

    // Technically this is unordered, but since we know Int32's hash function for positive integers it itself, we can consider this a test
    String s2 = format(Map, map);
    ASSERT_TRUE(str_equals_content(s2, "Map<Int32, Int32>{0: 1234, 1: 4321}"), "Actual: '%s'", s2->slice);
    del(String, s2);

    del(Map, map);
});

TEST(test_map_stress, {
    Map map = new(Map, 2, class(Int32), class(Int32));

    // Many insertions, replacements, and queries
    for (int32_t k = 0; k < 10; k++)
    {
        for (int32_t i = 0; i < 1000; i++)
        {
            for (int32_t j = 0; j < 10; j++)
            {
                Int32 key = new(Int32, (i + 11 * j + 52 * k) % 300);
                Int32 value = new(Int32, (i + 10 * j * 19 * k) % 300);

                map_put(map, key, value);
                Int32 get_value = map_get(map, key);

                ASSERT_TRUE(equals(Int32, get_value, value), "Key = %d, Value = %d, Get Value = %d. Map = %s", *key, *value, *get_value, format(Map, map)->slice);
            }
        }
        map_clear(map);
    }

    del(Map, map);
});

TEST_GROUP(test_map, {
    test_map_new();
    test_map_put_get();
    test_map_put_replace();
    test_map_format();

    test_map_stress();
});