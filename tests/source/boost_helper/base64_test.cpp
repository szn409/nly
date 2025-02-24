#include "gtest/gtest.h"
#include "nly/boost_helper/base64.hpp"

TEST(Base64, All)
{
  auto fun = [](const std::vector<unsigned char>& input)
  {
    ;
    std::vector<unsigned char> decode_output;

    std::string                encode_output = nly::boost_base64::encode(input.data(), input.size());
    auto len = nly::boost_base64::decode(encode_output.c_str(), encode_output.size(), decode_output);
    EXPECT_EQ(input, decode_output);
    auto index = encode_output.find('=');
    EXPECT_TRUE(len == (index == static_cast<size_t>(-1) ? encode_output.size() : index));
  };

  fun(std::vector<unsigned char>{ 0xAB });
  fun(std::vector<unsigned char>{ 0xAB, 0xCD });
  fun(std::vector<unsigned char>{ 0xAB, 0xCD, 0x00 });
  fun(std::vector<unsigned char>{ 0xAB, 0xCD, 0x00, 0x11 });
  fun(std::vector<unsigned char>{ 0xAB, 0xCD, 0x00, 0x11, 0x12 });
  fun(std::vector<unsigned char>{ 0xAB, 0xCD, 0x00, 0x11, 0x12, 0xFF });
  fun(std::vector<unsigned char>{ 'h', 'e', 'l', 'l', 'o' });
}