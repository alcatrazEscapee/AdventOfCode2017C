
TEST(test_int_array_list_new, {
    IntArrayList* list = new(IntArrayList, 10);
    del(IntArrayList, list);
});

TEST(test_int_array_list_copy, {
    IntArrayList* list = new(IntArrayList, 10);
    ial_append(list, 3);
    ial_append(list, 6);
    ial_append(list, 2);

    ASSERT_EQUAL(list->length, 3, "Length is %d", list->length);

    IntArrayList* other = copy(IntArrayList, list);

    ASSERT_EQUAL(other->length, 3, "Other Length is %d", other->length);
    
    for (int32_t i = 0; i < 3; i++)
    {
        ASSERT_EQUAL(list->values[i], other->values[i], "Copy not equal at index %d, value %d != %d", i, list->values[i], other->values[i]);
    }

    del(IntArrayList, list);
    del(IntArrayList, other);
});

TEST(test_int_array_list_format, {
    IntArrayList* list = new(IntArrayList, 10);

    String* s1 = format(IntArrayList, list);
    ASSERT_TRUE(str_equals_content(s1, "IntArrayList{}"), "Got: %s", s1->slice);

    ial_append(list, 3);
    ial_append(list, 6);
    ial_append(list, 2);

    String* s2 = format(IntArrayList, list);
    ASSERT_TRUE(str_equals_content(s2, "IntArrayList{3, 6, 2}"), "Got: %s", s2->slice);
    
    del(String, s1);
    del(String, s2);
    del(IntArrayList, list);
});

TEST(test_int_array_list_append_get_set, {
    IntArrayList* list = new(IntArrayList, 10);

    ASSERT_EQUAL(list->length, 0, "Length is %d", list->length);

    ial_append(list, 3);
    ial_append(list, 6);
    ial_append(list, 2);

    ASSERT_EQUAL(list->length, 3, "Length is %d", list->length);

    ial_set(list, 2, -6);

    ASSERT_EQUAL(list->length, 3, "Length is %d", list->length);
    ASSERT_EQUAL(list->values[2], -6, "Value is %d", list->values[2]);

    int32_t v = ial_get(list, 1);

    ASSERT_EQUAL(list->length, 3, "Length is %d", list->length);
    ASSERT_EQUAL(v, 6, "Value is %d", v);
    
    del(IntArrayList, list);
});

TEST(test_int_array_list_resize, {
    IntArrayList* list = new(IntArrayList, 2);

    for (int32_t i = 0; i < 100; i++)
    {
        ial_append(list, i * i);
    }

    ASSERT_EQUAL(list->length, 100, "Length is %d", list->length);

    for (int32_t i = 0; i < 100; i++)
    {
        int32_t v = ial_get(list, i);
        ASSERT_EQUAL(v, i * i, "Valid at index %d is %d", i, v);
    }
    
    del(IntArrayList, list);
});

TEST(test_int_array_list_iterator, {
    IntArrayList* list = new(IntArrayList, 20);
    for (int32_t i = 0; i < 100; i++)
    {
        ial_append(list, i * i);
    }

    int32_t i = 0;
    for iter(IntArrayList, it, list)
    {
        ASSERT_TRUE((uint32_t) i < list->length, "Iterator exceeded list length: %d >= %d", i, list->length);
        ASSERT_EQUAL((uint32_t) i, it.index, "Iterator index was %d, expected %d", it.index, i);
        ASSERT_EQUAL(i * i, it.value, "Iterator value was %d, expected %d", it.value, i * i);
        i++;
    }

    del(IntArrayList, list);
});

TEST(test_int_array_list_clear, {
    IntArrayList* list = new(IntArrayList, 10);
    ial_append(list, 3);
    ial_append(list, 6);
    ial_append(list, 2);
    ial_clear(list);

    ASSERT_EQUAL(list->length, 0, "Length was %d", list->length);
    
    del(IntArrayList, list);
});

TEST(test_int_array_list_equals, {
    IntArrayList* list = new(IntArrayList, 10);
    ial_append(list, 3);
    ial_append(list, 6);
    ial_append(list, 2);

    IntArrayList* other = copy(IntArrayList, list);

    ASSERT_TRUE(equals(IntArrayList, list, other), "Copy should be equal");

    ial_append(list, 5);

    ASSERT_FALSE(equals(IntArrayList, list, other), "Inequal sizes should not be equal");
    ASSERT_FALSE(equals(IntArrayList, other, list), "Inequal sizes should not be equal"); // Reverse order

    ial_append(other, 6);

    ASSERT_FALSE(equals(IntArrayList, list, other), "Lists with different content should not be equal");

    del(IntArrayList, list);
    del(IntArrayList, other);
});

TEST(test_int_array_list_compare, {
    IntArrayList* first = new(IntArrayList, 10);
    IntArrayList* second = new(IntArrayList, 10);
    IntArrayList* third = new(IntArrayList, 10);

    ial_append(first, 3);

    ASSERT_EQUAL(compare(IntArrayList, first, second), 1, "Array list with more elements should be larger");
    ASSERT_EQUAL(compare(IntArrayList, second, first), -1, "Array list with more elements should be larger"); // Reverse order
    ASSERT_EQUAL(compare(IntArrayList, second, third), 0, "Copy should be equal");

    ial_append(second, 5);

    ASSERT_EQUAL(compare(IntArrayList, first, second), -1, "Smaller elements should be smaller");

    ial_append(first, 3);
    ial_append(second, 1);
    ial_append(third, 3);
    ial_append(third, 5);

    // First: [3, 3], Second: [5, 3], Third: [3, 5]

    ASSERT_EQUAL(compare(IntArrayList, first, second), -1, "Earlier smaller elements should be smaller");
    ASSERT_EQUAL(compare(IntArrayList, first, third), -1, "Earlier smaller elements should be smaller");
    ASSERT_EQUAL(compare(IntArrayList, second, third), 1, "Earlier smaller elements should be smaller");

    del(IntArrayList, first);
    del(IntArrayList, second);
    del(IntArrayList, third);
});

TEST_GROUP(test_int_array_list_main, {
    test_int_array_list_new();
    test_int_array_list_copy();
    test_int_array_list_format();
    test_int_array_list_append_get_set();
    test_int_array_list_resize();
    test_int_array_list_iterator();
    test_int_array_list_clear();
    test_int_array_list_equals();
    test_int_array_list_compare();
});