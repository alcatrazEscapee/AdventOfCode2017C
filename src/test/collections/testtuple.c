
#include "../unittest.h"

// First, create and include a couple tuple types. The test is that they compile successfully!

#define Tuple BoolBox, bool, value
#include "../../lib/collections/tuple.template.c"

#define Tuple IdString, String, name, uint32_t, id
#include "../../lib/collections/tuple.template.c"

#define Tuple Numbers, int32_t, i32, uint32_t, u32, int64_t, i64, uint64_t, u64
#include "../../lib/collections/tuple.template.c"

// Generic Tests

TEST(test_tuple_boolbox_new, {
    BoolBox box = new(BoolBox, false);
    del(BoolBox, box);
});

TEST(test_tuple_boolbox_format, {
    BoolBox box = new(BoolBox, false);
    String s = format(BoolBox, box);

    ASSERT_TRUE(str_equals_content(s, "BoolBox<bool>(false)"), "Actual: '%s'", s->slice);

    del(String, s);
    del(BoolBox, box);
});

TEST(test_tuple_idstring_new, {
    IdString id = new(IdString, new(String, "three"), 3);
    del(IdString, id);
});

TEST(test_tuple_idstring_format, {
    IdString id = new(IdString, new(String, "three"), 3);
    String s = format(IdString, id);

    ASSERT_TRUE(str_equals_content(s, "IdString<String, uint32_t>(three, 3)"), "Actual: '%s'", s->slice);

    del(String, s);
    del(IdString, id);
});

TEST(test_tuple_numbers_new, {
    Numbers n = new(Numbers, -10, 10, -100, 100);
    del(Numbers, n);
});

TEST(test_tuple_numbers_format, {
    Numbers n = new(Numbers, -10, 10, -100, 100);
    String s = format(Numbers, n);

    ASSERT_TRUE(str_equals_content(s, "Numbers<int32_t, uint32_t, int64_t, uint64_t>(-10, 10, -100, 100)"), "Actual: '%s'", s->slice);

    del(String, s);
    del(Numbers, n);
});


TEST_GROUP(test_tuple, {
    test_tuple_boolbox_new();
    test_tuple_boolbox_format();

    test_tuple_idstring_new();
    test_tuple_idstring_format();

    test_tuple_numbers_new();
    test_tuple_numbers_format();
});
