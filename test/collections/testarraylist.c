
TEST(test_array_list_new, {
    ArrayList* list = new(ArrayList, 10, class(String));
    del(ArrayList, list);
});

TEST(test_array_list_copy, {
    ArrayList* list = new(ArrayList, 10, class(String));
    al_append(list, new(String, "first"));
    al_append(list, new(String, "second"));
    al_append(list, new(String, "third"));

    ASSERT_EQUAL(list->length, 3, "Length is %d", list->length);

    ArrayList* other = copy(ArrayList, list);

    ASSERT_EQUAL(other->length, 3, "Other Length is %d", other->length);
    
    for (int32_t i = 0; i < 3; i++)
    {
        ASSERT_TRUE(equals(String, list->values[i], other->values[i]), "Copy not equal at index %d, value %s != %s", i, ((String*)list->values[i])->slice, ((String*)other->values[i])->slice);
    }

    del(ArrayList, list);
    del(ArrayList, other);
});

TEST(test_array_list_format, {
    ArrayList* list = new(ArrayList, 10, class(String));

    String* s1 = format(ArrayList, list);
    ASSERT_TRUE(str_equals_content(s1, "ArrayList{}"), "Got: %s", s1->slice);

    al_append(list, new(String, "first"));
    al_append(list, new(String, "second"));
    al_append(list, new(String, "third"));

    String* s2 = format(ArrayList, list);
    ASSERT_TRUE(str_equals_content(s2, "ArrayList{first, second, third}"), "Got: %s", s2->slice);
    
    del(String, s1);
    del(String, s2);
    del(ArrayList, list);
});

TEST(test_array_list_append_get_set, {
    ArrayList* list = new(ArrayList, 10, class(String));

    ASSERT_EQUAL(list->length, 0, "Length is %d", list->length);

    al_append(list, new(String, "first"));
    al_append(list, new(String, "second"));
    al_append(list, new(String, "third"));

    ASSERT_EQUAL(list->length, 3, "Length is %d", list->length);

    al_set(list, 2, new(String, "replacement third"));

    ASSERT_EQUAL(list->length, 3, "Length is %d", list->length);
    ASSERT_TRUE(str_equals_content(list->values[2], "replacement third"), "Value is %s", ((String*)list->values[2])->slice);

    String* v = al_get(list, 1);

    ASSERT_EQUAL(list->length, 3, "Length is %d", list->length);
    ASSERT_TRUE(str_equals_content(v, "second"), "Value is %s", v->slice);
    
    del(ArrayList, list);
});

TEST(test_array_list_resize, {
    ArrayList* list = new(ArrayList, 2, class(String));

    for (int32_t i = 0; i < 100; i++)
    {
        al_append(list, str_format("numbas %d", i * i));
    }

    ASSERT_EQUAL(list->length, 100, "Length is %d", list->length);

    for (int32_t i = 0; i < 100; i++)
    {
        String* v = al_get(list, i);
        String* s = str_format("numbas %d", i * i);
        ASSERT_TRUE(equals(String, s, v), "Value at index %d is %s", i, ((String*)v)->slice);
        del(String, s);
    }
    
    del(ArrayList, list);
});

TEST(test_array_list_iterator, {
    ArrayList* list = new(ArrayList, 20, class(String));
    for (int32_t i = 0; i < 100; i++)
    {
        al_append(list, str_format("numbas %d", i * i));
    }

    int32_t i = 0;
    for iter(ArrayList, it, list)
    {
        String* s = str_format("numbas %d", i * i);
        ASSERT_TRUE((uint32_t) i < list->length, "Iterator exceeded list length: %d >= %d", i, list->length);
        ASSERT_EQUAL((uint32_t) i, it.index, "Iterator index was %d, expected %d", it.index, i);
        ASSERT_TRUE(equals(String, s, it.value), "Iterator value was %s, expected %s", ((String*)it.value)->slice, s->slice);
        i++;
        del(String, s);
    }

    // Generic type'd iterator
    i = 0;
    for type_iter(ArrayList, String*, it, list)
    {
        String* s = str_format("numbas %d", i * i);
        ASSERT_TRUE((uint32_t) i < list->length, "Iterator exceeded list length: %d >= %d", i, list->length);
        ASSERT_EQUAL((uint32_t) i, it.parent.index, "Iterator index was %d, expected %d", it.parent.index, i);
        ASSERT_TRUE(equals(String, s, it.value), "Iterator value was %s, expected %s", it.value->slice, s->slice); // No cast required from it.value to String* here
        i++;
        del(String, s);
    }

    del(ArrayList, list);
});

TEST(test_array_list_clear, {
    ArrayList* list = new(ArrayList, 10, class(String));
    al_append(list, new(String, "first"));
    al_append(list, new(String, "second"));
    al_append(list, new(String, "third"));
    al_clear(list);

    ASSERT_EQUAL(list->length, 0, "Length was %d", list->length);
    
    del(ArrayList, list);
});

TEST_GROUP(test_array_list_main, {
    test_array_list_new();
    test_array_list_copy();
    test_array_list_format();
    test_array_list_append_get_set();
    test_array_list_resize();
    test_array_list_iterator();
    test_array_list_clear();
});