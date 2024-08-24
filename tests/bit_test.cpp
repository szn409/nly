#include "gtest/gtest.h"
#include "nly/bit.hpp"

TEST(Bit, byteswap)
{
  EXPECT_TRUE(0x12 == nly::byteswap(static_cast<char>(0x12)));
  EXPECT_TRUE(0x3412 == nly::byteswap(static_cast<short>(0x1234)));
  EXPECT_TRUE(0x34120000 == nly::byteswap(0x1234));
  EXPECT_TRUE(0xCCDDEEFF == nly::byteswap(0xFFEEDDCC));
  EXPECT_TRUE(0xCCDDEEFF00000000 == nly::byteswap(0xFFEEDDCCLL));
}

TEST(Bit, GetBitValue)
{
  // 0xC5: 1100 0101
  // 0xAF: 1010 1111
  // 0x37: 0011 0111
  // 0xFF: 1111 1111
  // 0x00: 0000 0000
  // 0xF0: 1111 0000
  // 0x0F: 0000 1111
  // 0x01: 0000 0001
  // 0xAB: 1010 1011
  unsigned char buff[9] = { 0xC5, 0xAF, 0x37, 0xFF, 0x00, 0xF0, 0x0F, 0x01, 0xAB };

  EXPECT_TRUE(nly::get_bit_value(buff, 0, 3) == 6);
  EXPECT_TRUE(nly::get_bit_value(buff, 1, 3) == 4);
  EXPECT_TRUE(nly::get_bit_value(buff, 2, 7) == 11);
  EXPECT_TRUE(nly::get_bit_value(buff, 3, 9) == 90);
  EXPECT_TRUE(nly::get_bit_value(buff, 4, 5) == 11);
  EXPECT_TRUE(nly::get_bit_value(buff, 5, 15) == 23283);
  EXPECT_TRUE(nly::get_bit_value(buff, 6, 2) == 1);
  EXPECT_TRUE(nly::get_bit_value(buff, 7, 2) == 3);
  EXPECT_TRUE(nly::get_bit_value(buff, 8, 8) == 175);
  EXPECT_TRUE(nly::get_bit_value(buff, 9, 44) == 6489693494785LL);
  EXPECT_TRUE(nly::get_bit_value(buff, 10, 56) == 53163569109285894LL);

  EXPECT_TRUE(nly::get_bit_value(buff, 0, 8) == 0xC5);
  EXPECT_TRUE(nly::get_bit_value(buff, 0, 16) == 0xC5AF);
  EXPECT_TRUE(nly::get_bit_value(buff, 0, 32) == 0xC5AF37FF);
  EXPECT_TRUE(nly::get_bit_value(buff, 0, 64) == 0xC5AF37FF00F00F01);

  EXPECT_TRUE(nly::get_bit_value(buff, 1, 8) == 139);
  EXPECT_TRUE(nly::get_bit_value(buff, 1, 16) == 35678);
  EXPECT_TRUE(nly::get_bit_value(buff, 1, 32) == 0xC5AF37FF << 1);
  EXPECT_TRUE(nly::get_bit_value(buff, 1, 64) == (0xC5AF37FF00F00F01 << 1) + 1);
}

TEST(Bit, HexToStr)
{
  unsigned char input[] = { 0x01, 0x12, 0xAB, 0xFF };
  EXPECT_TRUE(nly::hex_to_str(input, 0) == "");
  EXPECT_TRUE(nly::hex_to_str(input, 1) == "01");
  EXPECT_TRUE(nly::hex_to_str(input, 4) == "01 12 AB FF");
  EXPECT_TRUE(nly::hex_to_str(input, 2, "") == "0112");
  EXPECT_TRUE(nly::hex_to_str(input, 2, "*") == "01*12");
  EXPECT_TRUE(nly::hex_to_str(input, 2, "**") == "01**12");
  EXPECT_TRUE(nly::hex_to_str(input, 3, " ", false) == "01 12 ab");
  EXPECT_TRUE(nly::hex_to_str(input, 3, "----", false) == "01----12----ab");

  std::string str;
  str.push_back(0xEE);
  str.push_back(0xFF);
  unsigned char out[] = { '0', '1', 0xEE, 0xFF, '1', '2' };
  EXPECT_TRUE(!memcmp(out, nly::hex_to_str(input, 2, str).c_str(), 6));
}

TEST(Bit, StrToHex)
{
  std::vector<unsigned char> out;

  auto fun = [&out](const std::string& input, const std::vector<unsigned char>& output)
  {
    nly::str_to_hex(input, out);
    EXPECT_EQ(out, output);
  };

  fun("", {});
  fun("   ", {});
  fun("f", { 0x0f });
  fun(" f  ", { 0x0f });
  fun("fa", { 0xfa });
  fun("  f a ", { 0xfa });
  fun("fab", { 0x0f, 0xab });
  fun("fabc", { 0xfa, 0xbc });
  fun(" f a b c   ", { 0xfa, 0xbc });
  fun(" f a b c   d", { 0x0f, 0xab, 0xcd });
}

TEST(Bit, From10BitTo16Bit)
{
  // 0xC5: 1100 0101
  // 0xAF: 1010 1111
  // 0x37: 0011 0111
  // 0xFF: 1111 1111
  // 0xAB: 1010 1011
  unsigned char  buff[5] = { 0xC5, 0xAF, 0x37, 0xFF, 0xAB };
  unsigned short out[4] = {};

  nly::from_10bit_to_16bit(buff, 5, out);
  EXPECT_EQ(out[0], 790);
  EXPECT_EQ(out[1], 755);
  EXPECT_EQ(out[2], 511);
  EXPECT_EQ(out[3], 939);
}

TEST(Bit, From12BitTo16Bit)
{
  // 0xC5: 1100 0101
  // 0xAF: 1010 1111
  // 0x37: 0011 0111
  unsigned char  buff[3] = { 0xC5, 0xAF, 0x37 };
  unsigned short out[2] = {};

  nly::from_12bit_to_16bit(buff, 3, out);
  EXPECT_EQ(out[0], 3162);
  EXPECT_EQ(out[1], 3895);
}

TEST(Bit, From16BitTo8Bit)
{
  // 0xC5: 1100 0101
  // 0xAF: 1010 1111
  unsigned short input = 0xC5AF;
  unsigned char  out = {};

  auto fun = [&input, &out](int mode, int expect)
  {
    nly::from_16bit_to_8bit(&input, 1, &out, mode);
    EXPECT_EQ(out, expect);
  };

  fun(0, 175);
  fun(1, 215);
  fun(2, 107);
  fun(3, 181);
  fun(4, 90);
  fun(5, 45);
  fun(6, 22);
  fun(7, 139);
  fun(8, 197);
}