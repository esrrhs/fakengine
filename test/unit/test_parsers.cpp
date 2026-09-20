#include <gtest/gtest.h>
#include "fakengine.h"

TEST(ParsersTest, TrieInsertAndCheck) {
    ftrie<uint8_t, 100, 100> trie;
    trie.clear();

    trie.insert(stringc("badword"));
    trie.insert(stringc("spam"));

    fstring<uint8_t, c_DefaultStringBuffer> input("this is a badword here");
    fstring<uint8_t, c_DefaultStringBuffer> replaced = input;
    size_t count = trie.filterword(replaced, '*');

    EXPECT_GT(count, 0u);
    EXPECT_NE(strstr((const char*)replaced.c_str(), "*******"), nullptr);
}

TEST(ParsersTest, IniFileParseFromString) {
    inifile<10, 10> ini;
    fstring<uint8_t, c_IniFileBufferSize> buff(
        "[server]\n"
        "ip = 127.0.0.1;\n"
        "port = 8080;\n"
        "[client]\n"
        "timeout = 30;\n"
    );

    ASSERT_TRUE(ini.parse(buff));

    stringc ip;
    ASSERT_TRUE(ini.get("server", "ip", ip));
    EXPECT_STREQ((const char*)ip.c_str(), "127.0.0.1");

    int32_t port = 0;
    ASSERT_TRUE(ini.get("server", "port", port));
    EXPECT_EQ(port, 8080);

    int32_t timeout = 0;
    ASSERT_TRUE(ini.get("client", "timeout", timeout));
    EXPECT_EQ(timeout, 30);
}
