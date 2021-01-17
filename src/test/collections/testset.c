
#include "../unittest.h"

TEST(test_set_new, {
    Set set = new(Set, 10, class(Int32));
    del(Set, set);
});

TEST(test_set_put_contains, {
    Set set = new(Set, 10, class(Int32));
    Int32 key = new(Int32, 2);
    Int32 bad_key = new(Int32, 5);

    set_put(set, copy(Int32, key));

    ASSERT_TRUE(set_contains(set, key), "Set should contain key = 2");
    ASSERT_FALSE(set_contains(set, bad_key), "Set should not contain key = 5");

    del(Int32, key);
    del(Int32, bad_key);
    del(Set, set);
});

TEST(test_set_put_replace, {
    Set set = new(Set, 10, class(Int32));
    Int32 key = new(Int32, 2);

    set_put(set, copy(Int32, key));
    set_put(set, copy(Int32, key));

    ASSERT_TRUE(set_contains(set, key), "Set should contain key = 2");

    del(Int32, key);
    del(Set, set);
});

TEST(test_set_format, {
    Set set = new(Set, 10, class(Int32));

    String s1 = format(Set, set);
    ASSERT_TRUE(str_equals_content(s1, "Set<Int32>{}"), "Actual: '%s'", s1->slice);
    del(String, s1);

    set_put(set, new(Int32, 0));
    set_put(set, new(Int32, 1));

    // Technically this is unordered, but since we know Int32's hash function for positive integers is itself, we can consider this a test
    String s2 = format(Set, set);
    ASSERT_TRUE(str_equals_content(s2, "Set<Int32>{0, 1}"), "Actual: '%s'", s2->slice);
    del(String, s2);

    del(Set, set);
});

TEST(test_set_stress, {
    Set set = new(Set, 2, class(Int32));

    // Many insertions, replacements, and queries
    for (int32_t k = 0; k < 10; k++)
    {
        for (int32_t i = 0; i < 1000; i++)
        {
            for (int32_t j = 0; j < 10; j++)
            {
                // Key is moved to the map, but the value is still allowed to be borrowed here
                Int32 key = new(Int32, (i + 11 * j + 52 * k) % 300);
                set_put(set, key);

                ASSERT_TRUE(set_contains(set, key), "Key = %d. Set = %s", *key, format(Set, set)->slice);
            }
        }
        set_clear(set);
    }

    del(Set, set);
});

TEST_GROUP(test_set, {
    test_set_new();
    test_set_put_contains();
    test_set_put_replace();
    test_set_format();

    test_set_stress();
});