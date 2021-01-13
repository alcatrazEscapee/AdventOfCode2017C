
#include "unittest.h"

TEST(test_strings_str_format, {
    String s1 = str_format("test format %d", 100);
    ASSERT_TRUE(str_equals_content(s1, "test format 100"), "Got '%s' instead", s1->slice);
    ASSERT_EQUAL(s1->length, 15, "Expected length 15, got %d", s1->length);

    String s2 = str_format("->%s<- %d?", "long string that really shouldn't fit", 100);
    ASSERT_TRUE(str_equals_content(s2, "->long string that really shouldn't fit<- 100?"), "Got '%s' instead", s2->slice);
    ASSERT_EQUAL(s2->length, 46, "Expected length 46, got %d", s2->length);

    del(String, s1);
    del(String, s2);
});

TEST(test_strings_str_slice_len, {
    ASSERT_EQUAL(str_slice_len("15LengthString!"), 15, "?");
    ASSERT_EQUAL(str_slice_len("1"), 1, "?");
    ASSERT_EQUAL(str_slice_len(""), 0, "?");
});

TEST(test_strings_new, {
    String s = new(String, "Hello World!");
    del(String, s);
});

TEST(test_strings_str_equals, {
    String s1 = new(String, "Hello World");
    String s2 = new(String, "Hello World");
    String s3 = new(String, "Not Hello World?");
    String s4 = new(String, "Hello Level");

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
    String s1 = new(String, "abc");
    String s2 = new(String, "abb");
    String s3 = new(String, "abbx");

    ASSERT_EQUAL(compare(String, s1, s1), 0, "Compare equal strings should be 0");
    ASSERT_EQUAL(compare(String, s1, s2), 1, "s2 is ordered before s1");
    ASSERT_EQUAL(compare(String, s2, s1), -1, "s1 is ordered before s1");
    ASSERT_EQUAL(compare(String, s1, s3), 1, "s3 is ordered before s1 despite being longer");
    ASSERT_EQUAL(compare(String, s2, s3), -1, "s2 is ordered before s3 as it's shorter");

    del(String, s1);
    del(String, s2);
    del(String, s3);
});

TEST(test_strings_iter, {
    String s = new(String, "abcd");
    uint32_t i = 0;
    slice_t exp = "abcd";
    for iter(String, it, s)
    {
        ASSERT_EQUAL(it.value, exp[i], "iter() index %d is %c", i, it.value);
        i++;
    }

    del(String, s);
});

TEST(test_strings_iter_split, {
    String s = new(String, "abc?acd!gdc");
    uint32_t i = 0;
    slice_t exp[] = {"abc", "acd", "gdc"};
    for iter(StringSplit, it, s, "?!")
    {
        ASSERT_TRUE(str_equals_content(it.value, exp[i]), "iter() index %d is %s", i, it.value->slice);
        i++;
    }

    del(String, s);
});

TEST(test_strings_str_append_char, {
    String s1 = new(String, "test");

    ASSERT_EQUAL(s1->length, 4, "Expected length 4, got %d", s1->length);

    str_append_char(s1, '1');
    ASSERT_TRUE(str_equals_content(s1, "test1"), "Expected s1 = 'test1', got '%s'", s1->slice);
    ASSERT_EQUAL(s1->length, 5, "Expected length 5, got %d", s1->length);

    for (uint32_t i = 0; i < 16; i++)
    {
        str_append_char(s1, '2');
    }

    ASSERT_TRUE(str_equals_content(s1, "test12222222222222222"), "16 '2's should be appended to 'test1', got '%s'", s1->slice);
    ASSERT_EQUAL(s1->length, 21, "Expected length 5, got %d", s1->length);

    del(String, s1);
});

TEST(test_strings_str_append_slice, {
    String s1 = new(String, "test");

    ASSERT_EQUAL(s1->length, 4, "Expected length 4, got %d", s1->length);

    str_append_slice(s1, " and some more text");
    ASSERT_TRUE(str_equals_content(s1, "test and some more text"), "String not equal: '%s'", s1->slice);
    ASSERT_EQUAL(s1->length, 23, "Expected length 23, got %d", s1->length);

    del(String, s1);
});

TEST(test_strings_str_append_string, {
    String s1 = new(String, "test");

    str_append_string(s1, new(String, " and some more text")); // Consumes argument
    ASSERT_TRUE(str_equals_content(s1, "test and some more text"), "String not equal: '%s'", s1->slice);
    ASSERT_EQUAL(s1->length, 23, "Expected length 23, got %d", s1->length);

    del(String, s1);
});

TEST(test_strings_str_equals_content, {
    String s1 = new(String, "test some things");

    ASSERT_TRUE(str_equals_content(s1, "test some things"), "Content should equal");
    ASSERT_FALSE(str_equals_content(s1, "test some things and others"), "Static string is longer");
    ASSERT_FALSE(str_equals_content(s1, "test some"), "Static string is shorter");
    ASSERT_FALSE(str_equals_content(s1, "test other things"), "Content should not equal");

    del(String, s1);
});

TEST(test_strings_str_substring, {
    String s1 = new(String, "the lazy cat");

    String s2 = str_substring(s1, 0, s1->length); // copy
    ASSERT_TRUE(equals(String, s1, s2), "Expected a copy, got '%s'", s2->slice);

    String s3 = str_substring(s1, 0, 3);
    ASSERT_TRUE(str_equals_content(s3, "the"), "Expected the first word, got '%s'", s3->slice);

    String s4 = str_substring(s1, 4, 8);
    ASSERT_TRUE(str_equals_content(s4, "lazy"), "Expected the second word, got '%s'", s4->slice);

    String s5 = str_substring(s1, 9, s1->length);
    ASSERT_TRUE(str_equals_content(s5, "cat"), "Expected the third word, got '%s'", s5->slice);

    del(String, s1);
    del(String, s2);
    del(String, s3);
    del(String, s4);
    del(String, s5);
});

TEST(test_strings_str_sort, {
    String s1 = new(String, "thelazycat");
    str_sort(s1);
    ASSERT_TRUE(str_equals_content(s1, "aacehlttyz"), "Expected sorted, got '%s'", s1->slice);

    String s2 = new(String, "qwertyuioplkjhgfdsazxcvbnm");
    str_sort(s2);
    ASSERT_TRUE(str_equals_content(s2, "abcdefghijklmnopqrstuvwxyz"), "Expected sorted, got '%s'", s2->slice);

    del(String, s1);
    del(String, s2);
});

TEST_GROUP(test_strings, {
    test_strings_str_format();
    test_strings_str_slice_len();
    test_strings_new();
    test_strings_str_equals();
    test_strings_str_compare();
    test_strings_iter();
    test_strings_iter_split();
    test_strings_str_append_char();
    test_strings_str_append_slice();
    test_strings_str_append_string();
    test_strings_str_equals_content();
    test_strings_str_substring();
    test_strings_str_sort();
});
