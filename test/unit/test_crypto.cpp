#include <gtest/gtest.h>
#include "fakengine.h"

TEST(CryptoTest, MD5StandardVector) {
    // fmd5 returns uppercase hex
    stringc empty_hash = fmd5((const uint8_t*)"", 0);
    EXPECT_STREQ((const char*)empty_hash.c_str(), "D41D8CD98F00B204E9800998ECF8427E");

    stringc abc_hash = fmd5((const uint8_t*)"abc", 3);
    EXPECT_STREQ((const char*)abc_hash.c_str(), "900150983CD24FB0D6963F7D28E17F72");
}

TEST(CryptoTest, SHA1StandardVector) {
    // fsha1 returns uppercase hex
    stringc empty_hash = fsha1((const uint8_t*)"", 0);
    EXPECT_STREQ((const char*)empty_hash.c_str(), "DA39A3EE5E6B4B0D3255BFEF95601890AFD80709");

    stringc abc_hash = fsha1((const uint8_t*)"abc", 3);
    EXPECT_STREQ((const char*)abc_hash.c_str(), "A9993E364706816ABA3E25717850C26C9CD0D89D");
}

TEST(CryptoTest, CRC32StandardVector) {
    // CRC32 of "123456789" = 0xcbf43926
    const uint8_t data[] = "123456789";
    uint32_t crc = fcrc32a(data, 9);
    EXPECT_EQ(crc, 0xcbf43926);
}
