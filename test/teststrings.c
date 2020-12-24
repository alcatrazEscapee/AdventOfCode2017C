// Tests for strings.c, strings.h

#include "../src/strings.h"

TEST(test_strings_str_format, {
    String* s1 = str_format("test format %d", 100);
    ASSERT_TRUE(str_equals_content(s1, "test format 100"), "Got '%s' instead", s1->slice);

    String* s2 = str_format("->%s<- %d?", "long string that really shouldn't fit", 100);
    ASSERT_TRUE(str_equals_content(s2, "->long string that really shouldn't fit<- 100?"), "Got '%s' instead", s2->slice);

    del(String, s1);
    del(String, s2);
});

TEST(test_strings_str_slice_len, {
    ASSERT_EQUAL(str_slice_len("15LengthString!"), 15, "?");
});

TEST(test_strings_new, {
    String* s = new(String, "Hello World!");
    del(String, s);
});

TEST(test_strings_str_equals, {
    String* s1 = new(String, "Hello World");
    String* s2 = new(String, "Hello World");
    String* s3 = new(String, "Not Hello World?");
    String* s4 = new(String, "Hello Level");

    ASSERT_TRUE(s1 != s2, "Strings should not be reference equal");
    ASSERT_TRUE(equals(String, s1, s2), "Strings should be equal using equals()");
    ASSERT_TRUE(equals_c(class(String), s1, s2), "Strings should be equal using equals_c(class())");

    ASSERT_FALSE(equals(String, s2, s3), "Should not be equal on length");
    ASSERT_FALSE(equals(String, s2, s4), "Should not be equal on content");

    del(String, s1);
    del(String, s2);
    del(String, s3);
    del(String, s4);
});

TEST(test_strings_str_compare, {
    String* s1 = new(String, "abc");
    String* s2 = new(String, "abb");
    String* s3 = new(String, "abbx");

    ASSERT_EQUAL(compare(String, s1, s1), 0, "Compare equal strings should be 0");
    ASSERT_EQUAL(compare(String, s1, s2), 1, "s2 is ordered before s1");
    ASSERT_EQUAL(compare(String, s2, s1), -1, "s1 is ordered before s1");
    ASSERT_EQUAL(compare(String, s1, s3), 1, "s3 is ordered before s1 despite being longer");
    ASSERT_EQUAL(compare(String, s2, s3), -1, "s2 is ordered before s3 as it's shorter");

    del(String, s1);
    del(String, s2);
    del(String, s3);
});

TEST(test_strings_str_append_char, {
    String* s1 = new(String, "test");
    String* s2 = new(String, "test1");

    str_append_char(s1, '1');
    ASSERT_TRUE(equals(String, s1, s2), "'1' should be appended to 'test'");

    del(String, s1);
    del(String, s2);
});

TEST(test_strings_str_append_slice, {
    String* s1 = new(String, "test");
    String* s2 = new(String, "test and some more text");

    str_append_slice(s1, " and some more text");
    ASSERT_TRUE(equals(String, s1, s2), "' and some more text' should be appended to 'test'");

    del(String, s1);
    del(String, s2);
});

TEST(test_strings_str_append_string, {
    String* s1 = new(String, "test");
    String* s2 = new(String, "test and some more text");

    str_append_string(s1, new(String, " and some more text")); // Consumes argument
    ASSERT_TRUE(equals(String, s1, s2), "' and some more text' should be appended to 'test'");

    del(String, s1);
    del(String, s2);
});

TEST(test_strings_str_equals_content, {
    String* s1 = new(String, "test some things");

    ASSERT_TRUE(str_equals_content(s1, "test some things"), "Content should equal");
    ASSERT_FALSE(str_equals_content(s1, "test some things and others"), "Static string is longer");
    ASSERT_FALSE(str_equals_content(s1, "test some"), "Static string is shorter");
    ASSERT_FALSE(str_equals_content(s1, "test other things"), "Content should not equal");

    del(String, s1);
});

TEST_GROUP(test_strings_main, {
    test_strings_str_format();
    test_strings_str_slice_len();
    test_strings_new();
    test_strings_str_equals();
    test_strings_str_compare();
    test_strings_str_append_char();
    test_strings_str_append_slice();
    test_strings_str_append_string();
    test_strings_str_equals_content();
});