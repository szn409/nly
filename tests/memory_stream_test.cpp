#include "gtest/gtest.h"
#include "nly/memory_stream.hpp"
#include <bitset>

TEST(MemoryStream, BitSetCount)
{
  std::bitset<8> bs;

  for (int i = 0; i < 256; ++i)
  {
    bs = i;
    EXPECT_EQ(bs.count(), nly::bit_set_count[i]);
  }
}

TEST(MemoryStream, BitCmp)
{
  // 0001 0001
  // 1010 0101
  // 0100 1001
  unsigned char input[] = { 0x11, 0xA5, 0x49 };

  // 0001 0001
  // 1110 1101
  // 1100 0001
  unsigned char base[] = { 0x11, 0xED, 0xC1 };

  EXPECT_TRUE(nly::bit_cmp(input, base, 1));
  EXPECT_TRUE(nly::bit_cmp(input, base, 1, 2));
  EXPECT_TRUE(nly::bit_cmp(input, base, 1, 200));

  EXPECT_TRUE(!nly::bit_cmp(input, base, 2));
  EXPECT_TRUE(!nly::bit_cmp(input, base, 2, 1));
  EXPECT_TRUE(nly::bit_cmp(input, base, 2, 2));
  EXPECT_TRUE(nly::bit_cmp(input, base, 2, 1024));

  EXPECT_TRUE(!nly::bit_cmp(input, base, 3));
  EXPECT_TRUE(!nly::bit_cmp(input, base, 3, 1));
  EXPECT_TRUE(!nly::bit_cmp(input, base, 3, 2));
  EXPECT_TRUE(!nly::bit_cmp(input, base, 3, 3));
  EXPECT_TRUE(nly::bit_cmp(input, base, 3, 4));
  EXPECT_TRUE(nly::bit_cmp(input, base, 3, 2048));
}

TEST(MemoryStream, Left)
{
  nly::memory_stream ms([](const nly::memory_stream::memory_type& chunk) { delete[] chunk.first; });

  unsigned char output[10] = {};
  unsigned char target[10] = {};

  unsigned char* buf = new unsigned char[2];
  buf[0] = 0x01;
  buf[1] = 0x02;
  ms.add(buf, 2);
  EXPECT_EQ(ms.already_slide_byte(), 0);
  EXPECT_EQ(ms.available_byte(), 2);
  memset(output, 0, sizeof output);
  EXPECT_EQ(ms.peek(output, 0), 0);
  memset(output, 0, sizeof output);
  EXPECT_EQ(ms.peek(output, 1), 1);
  EXPECT_EQ(output[0], 0x01);
  memset(output, 0, sizeof output);
  EXPECT_EQ(ms.peek(output, 2), 2);
  EXPECT_EQ(output[0], 0x01);
  EXPECT_EQ(output[1], 0x02);
  memset(output, 0, sizeof output);
  EXPECT_EQ(ms.peek(output, 3), 2);
  EXPECT_EQ(output[0], 0x01);
  EXPECT_EQ(output[1], 0x02);
  memset(output, 0, sizeof output);
  EXPECT_EQ(ms.peek(output, 2, 1), 1);
  EXPECT_EQ(output[0], 0x02);
  EXPECT_EQ(ms.peek(output, 2, 2), 0);
  target[0] = 0x01;
  target[1] = 0x02;
  EXPECT_EQ(ms.find(target, 1), 0);
  EXPECT_EQ(ms.find(target, 2), 0);
  EXPECT_EQ(ms.find(target + 1, 1), 1);
  EXPECT_EQ(ms.find(target, 3), -1);
  target[0] = 0x11;
  EXPECT_EQ(ms.find(target, 1), -1);
  EXPECT_EQ(ms.find(target, 1, 1), 0);

  buf = new unsigned char[1];
  ms.add(buf, 0);
  EXPECT_EQ(ms.already_slide_byte(), 0);
  EXPECT_EQ(ms.available_byte(), 2);

  buf = new unsigned char[1];
  buf[0] = 0x03;
  ms.add(buf, 1);
  EXPECT_EQ(ms.already_slide_byte(), 0);
  EXPECT_EQ(ms.available_byte(), 3);
  memset(output, 0, sizeof output);
  EXPECT_EQ(ms.peek(output, 3), 3);
  EXPECT_EQ(output[0], 0x01);
  EXPECT_EQ(output[1], 0x02);
  EXPECT_EQ(output[2], 0x03);
  memset(output, 0, sizeof output);
  EXPECT_EQ(ms.peek(output, 3, 1), 2);
  EXPECT_EQ(output[0], 0x02);
  EXPECT_EQ(output[1], 0x03);
  memset(output, 0, sizeof output);
  EXPECT_EQ(ms.peek(output, 3, 2), 1);
  EXPECT_EQ(output[0], 0x03);
  EXPECT_EQ(ms.peek(output, 3, 3), 0);
  EXPECT_EQ(ms.peek(output, 4, 3), 0);
  EXPECT_EQ(ms.peek(output, 4), 3);

  buf = new unsigned char[3];
  buf[0] = 0x04;
  buf[1] = 0x05;
  buf[2] = 0x06;
  ms.add(buf, 3);
  EXPECT_EQ(ms.already_slide_byte(), 0);
  EXPECT_EQ(ms.available_byte(), 6);
  EXPECT_EQ(ms.slide(1), 1);
  EXPECT_EQ(ms.already_slide_byte(), 1);
  EXPECT_EQ(ms.available_byte(), 5);

  memset(output, 0, sizeof output);
  EXPECT_EQ(ms.peek(output, 10), 5);
  EXPECT_EQ(output[0], 0x02);
  EXPECT_EQ(output[1], 0x03);
  EXPECT_EQ(output[2], 0x04);
  EXPECT_EQ(output[3], 0x05);
  EXPECT_EQ(output[4], 0x06);
  EXPECT_EQ(ms.peek(output, 10, 1), 4);
  EXPECT_EQ(output[0], 0x03);
  EXPECT_EQ(output[1], 0x04);
  EXPECT_EQ(output[2], 0x05);
  EXPECT_EQ(output[3], 0x06);
  EXPECT_EQ(ms.peek(output, 10, 2), 3);
  EXPECT_EQ(output[0], 0x04);
  EXPECT_EQ(output[1], 0x05);
  EXPECT_EQ(output[2], 0x06);
  EXPECT_EQ(ms.peek(output, 10, 3), 2);
  EXPECT_EQ(output[0], 0x05);
  EXPECT_EQ(output[1], 0x06);
  EXPECT_EQ(ms.peek(output, 10, 4), 1);
  EXPECT_EQ(output[0], 0x06);
  EXPECT_EQ(ms.peek(output, 10, 5), 0);
  target[0] = 0x02;
  target[1] = 0x03;
  target[2] = 0x04;
  target[3] = 0x05;
  target[4] = 0x06;
  EXPECT_EQ(ms.find(target, 5), 0);
  EXPECT_EQ(ms.find(target + 1, 4), 1);
  EXPECT_EQ(ms.find(target + 2, 3), 2);
  EXPECT_EQ(ms.find(target + 3, 2), 3);
  EXPECT_EQ(ms.find(target + 4, 1), 4);

  EXPECT_EQ(ms.slide(2), 2);
  EXPECT_EQ(ms.already_slide_byte(), 3);
  EXPECT_EQ(ms.available_byte(), 3);
  EXPECT_EQ(ms.peek(output, 10), 3);
  EXPECT_EQ(output[0], 0x04);
  EXPECT_EQ(output[1], 0x05);
  EXPECT_EQ(output[2], 0x06);

  EXPECT_EQ(ms.slide(-1), 3);
  EXPECT_EQ(ms.slide(-1), 0);
}