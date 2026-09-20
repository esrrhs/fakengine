#include <gtest/gtest.h>
#include "fakengine.h"

TEST(FStringTest, ConstructionAndBasicProperties) {
    stringc str1;
    EXPECT_TRUE(str1.empty());
    EXPECT_EQ(str1.size(), 0u);
    EXPECT_STREQ((const char*)str1.c_str(), "");

    stringc str2("hello");
    EXPECT_FALSE(str2.empty());
    EXPECT_EQ(str2.size(), 5u);
    EXPECT_STREQ((const char*)str2.c_str(), "hello");

    stringc str3(str2);
    EXPECT_EQ(str3, str2);
    EXPECT_STREQ((const char*)str3.c_str(), "hello");

    stringc str4("hello world", 5);
    EXPECT_EQ(str4.size(), 5u);
    EXPECT_STREQ((const char*)str4.c_str(), "hello");
}

TEST(FStringTest, AppendAndConcatenate) {
    stringc str("foo");
    str += "bar";
    EXPECT_EQ(str.size(), 6u);
    EXPECT_STREQ((const char*)str.c_str(), "foobar");

    str.append("baz", 3);
    EXPECT_EQ(str.size(), 9u);
    EXPECT_STREQ((const char*)str.c_str(), "foobarbaz");

    str.append('!');
    EXPECT_EQ(str.size(), 10u);
    EXPECT_STREQ((const char*)str.c_str(), "foobarbaz!");
}

TEST(FStringTest, CaseConversion) {
    stringc lower("hello world");
    stringc upper = lower.make_upper();
    EXPECT_STREQ((const char*)upper.c_str(), "HELLO WORLD");

    stringc back_lower = upper.make_lower();
    EXPECT_STREQ((const char*)back_lower.c_str(), "hello world");

    EXPECT_TRUE(lower.equals_ignore_case(upper));
}

TEST(FStringTest, SearchAndSubString) {
    stringc str("the quick brown fox");
    int32_t pos = str.find("quick");
    EXPECT_EQ(pos, 4);

    int32_t pos_not_found = str.find("zebra");
    EXPECT_EQ(pos_not_found, -1);

    stringc sub = str.subString(4, 5);
    EXPECT_STREQ((const char*)sub.c_str(), "quick");

    int32_t last_o = str.findLast('o');
    EXPECT_GT(last_o, 0);
}

TEST(FStringTest, ReplaceAndTrim) {
    stringc str("  padded string  ");
    str.trim();
    EXPECT_STREQ((const char*)str.c_str(), "padded string");

    stringc repl("apple banana apple");
    repl.replace(stringc("apple"), stringc("orange"));
    EXPECT_STREQ((const char*)repl.c_str(), "orange banana orange");
}

TEST(FStringTest, Split) {
    stringc csv("one,two,three");
    farray<stringc, 10> tokens;
    csv.split(tokens, (const uint8_t*)",");
    ASSERT_EQ(tokens.size(), 3u);
    EXPECT_STREQ((const char*)tokens[0].c_str(), "one");
    EXPECT_STREQ((const char*)tokens[1].c_str(), "two");
    EXPECT_STREQ((const char*)tokens[2].c_str(), "three");
}
