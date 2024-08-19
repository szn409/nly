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