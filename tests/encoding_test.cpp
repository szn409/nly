#include "gtest/gtest.h"
#include "nly/encoding.hpp"

TEST(Encoding, All)
{
  auto str_u8 = u8"你";
  EXPECT_TRUE(static_cast<unsigned char>(str_u8[0]) == 228);
  EXPECT_TRUE(static_cast<unsigned char>(str_u8[1]) == 189);
  EXPECT_TRUE(static_cast<unsigned char>(str_u8[2]) == 160);

  // utf8 to gbk
  auto str_gbk = nly::from_utf(str_u8, str_u8 + 3, "gbk");
  EXPECT_TRUE(static_cast<unsigned char>(str_gbk->at(0)) == 196);
  EXPECT_TRUE(static_cast<unsigned char>(str_gbk->at(1)) == 227);

  // utf8 to big5
  auto str_big5 = nly::from_utf(str_u8, str_u8 + 3, "big5");
  EXPECT_TRUE(static_cast<unsigned char>(str_big5->at(0)) == 167);
  EXPECT_TRUE(static_cast<unsigned char>(str_big5->at(1)) == 65);

  // gbk to big5
  EXPECT_EQ(nly::between_non_utf(str_gbk->c_str(), str_gbk->c_str() + 2, "gbk", "big5"), str_big5);
  EXPECT_TRUE(nly::between_non_utf(str_gbk->c_str(), 2, "gbk", "big5") == str_big5);

  // gbk to utf8
  EXPECT_TRUE(nly::to_utf<char>(str_gbk->c_str(), str_gbk->c_str() + 2, "gbk") == str_u8);
  EXPECT_TRUE(nly::to_utf<char>(str_gbk->c_str(), 2, "gbk") == str_u8);

  // gbk to wchar_t
  auto str_w = nly::to_utf<wchar_t>(str_gbk->c_str(), 2, "gbk");
  EXPECT_TRUE(str_w->at(0) == 20320);

  // wchar_t to gbk
  EXPECT_TRUE(nly::from_utf(str_w->c_str(), str_w->c_str() + 1, "gbk") == str_gbk);

  // utf8 to utf16
  str_u8 = u8"👍";
  auto str_16 = nly::utf_to_utf<char16_t>(str_u8, str_u8 + 4);
  EXPECT_TRUE(static_cast<unsigned short>(str_16->at(0)) == 55357);
  EXPECT_TRUE(static_cast<unsigned short>(str_16->at(1)) == 56397);

  // utf8 to utf32
  auto str_32 = nly::utf_to_utf<char32_t>(str_u8, str_u8 + 4);
  EXPECT_TRUE(static_cast<unsigned int>(str_32->at(0)) == 128077);

  // utf16 to utf32
  EXPECT_TRUE(nly::utf_to_utf<char32_t>(str_16->c_str(), str_16->c_str() + 2) == str_32);

  // utf16 to utf8
  EXPECT_TRUE(nly::utf_to_utf<char>(str_16->c_str(), str_16->c_str() + 2) == str_u8);

  // utf32 to utf16
  EXPECT_TRUE(nly::utf_to_utf<char16_t>(str_32->c_str(), str_32->c_str() + 1) == str_16);

  // utf32 to utf8
  EXPECT_TRUE(nly::utf_to_utf<char>(str_32->c_str(), str_32->c_str() + 1) == str_u8);
}