#ifndef NLY_BOOST_HELPER_BASE64
#define NLY_BOOST_HELPER_BASE64

#include "boost/beast/core/detail/base64.hpp"
#include <string>
#include <vector>

namespace nly
{

class boost_base64
{
public:
  static void encode(const void* input, const size_t input_bytes, std::string& output)
  {
    output.resize(boost::beast::detail::base64::encoded_size(input_bytes));
    auto result = boost::beast::detail::base64::encode(output.data(), input, input_bytes);
    output.resize(result);
  }

  static std::string encode(const void* input, const size_t input_bytes)
  {
    std::string output;
    encode(input, input_bytes, output);
    return output;
  }

  /*
  返回值: 从 input 中读取的字节数
  注意: 在 base64 编码中, 字符 '=' 充当填充符, 所以返回值可能小于 input_bytes
  */
  static size_t decode(
    const char*                 input,
    const size_t                input_bytes,
    std::vector<unsigned char>& output)
  {
    output.resize(boost::beast::detail::base64::decoded_size(input_bytes));
    auto result = boost::beast::detail::base64::decode(output.data(), input, input_bytes);
    output.resize(result.first);
    return result.second;
  }

  static std::vector<unsigned char> decode(
    const char*  input,
    const size_t input_bytes,
    size_t*      real_read_bytes = nullptr)
  {
    std::vector<unsigned char> output;
    size_t                     value = decode(input, input_bytes, output);
    if (real_read_bytes)
    {
      *real_read_bytes = value;
    }
    return output;
  }
};

} // namespace nly

#endif
