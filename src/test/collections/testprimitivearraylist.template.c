// Template for PrimitiveArrayList(type) Tests
// @param type : The type of the list
// @param v1 : A literal of type 'type'
// @param v1s : (Optional) The string literal form of param 'v1'

#ifndef v1s
#define v1s LITERAL(v1)
#endif

TEST(CONCAT3(test_, type, _list_new), {
    PrimitiveArrayList(type) list = new(PrimitiveArrayList(type), 10);

    del(PrimitiveArrayList(type), list);
});

TEST(CONCAT3(test_, type, _list_format), {
    PrimitiveArrayList(type) list = new(PrimitiveArrayList(type), 10);

    String s1 = format(PrimitiveArrayList(type), list);
    ASSERT_TRUE(str_equals_content(s1, "PrimitiveArrayList<" LITERAL(type) ">{}"), "Actual: '%s'", s1->slice);

    al_append(list, v1);
    al_append(list, v1);

    String s2 = format(PrimitiveArrayList(type), list);
    ASSERT_TRUE(str_equals_content(s2, "PrimitiveArrayList<" LITERAL(type) ">{" v1s ", " v1s "}"), "Actual: '%s'", s2->slice);

    del(String, s1);
    del(String, s2);
    del(PrimitiveArrayList(type), list);
});

TEST(CONCAT3(test_, type, _list_append_get), {
    PrimitiveArrayList(type) list = new(PrimitiveArrayList(type), 10);

    al_append(list, v1);

    ASSERT_EQUAL(list->length, 1, "Actual length = %d", list->length);
    ASSERT_EQUAL(al_get(list, 0), v1, "List value not equal after append");

    del(PrimitiveArrayList(type), list);
});

TEST(CONCAT3(test_, type, _list_append_get_resize), {
    PrimitiveArrayList(type) list = new(PrimitiveArrayList(type), 1);

    for (uint32_t i = 0; i < 100; i++)
    {
        al_append(list, v1);
    }

    ASSERT_EQUAL(list->length, 100, "Actual length = %d", list->length);

    for (uint32_t i = 0; i < 100; i++)
    {
        ASSERT_EQUAL(al_get(list, i), v1, "List value not equal after append + resize");
    }

    del(PrimitiveArrayList(type), list);
});

TEST_GROUP(CONCAT3(test_, type, _list), {
    CONCAT3(test_, type, _list_new)();
    CONCAT3(test_, type, _list_format)();
    CONCAT3(test_, type, _list_append_get)();
    CONCAT3(test_, type, _list_append_get_resize)();
});

#undef type
#undef v1
#undef v1s
