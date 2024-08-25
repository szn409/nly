#ifndef NLY_BASE64
#define NLY_BASE64

#include "boost/beast/core/detail/base64.hpp"
#include <string>
#include <vector>

namespace nly
{

void base64_encode(const void* input, const size_t input_byte, std::string& output)
{
  output.resize(boost::beast::detail::base64::encoded_size(input_byte));
  auto result = boost::beast::detail::base64::encode(output.data(), input, input_byte);
  output.resize(result);
}

// Return the number of bytes read from the input.
// Note:
//  1. In Base64 encoding, the '=' character is used as a padding character.
//  2. So the return value may be smaller than input_byte.
size_t base64_decode(const char* input, const size_t input_byte, std::vector<unsigned char>& output)
{
  output.resize(boost::beast::detail::base64::decoded_size(input_byte));
  auto result = boost::beast::detail::base64::decode(output.data(), input, input_byte);
  output.resize(result.first);
  return result.second;
}

} // namespace nly

#endif // !NLY_BASE64
