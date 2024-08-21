#ifndef NLY_BIT
#define NLY_BIT

#include "boost/core/bit.hpp"
#include "fmt/format.h"
#include <optional>
#include <algorithm>

namespace nly
{

// T must be an integer type.
template<typename T>
T byteswap(T value)
{
  return boost::core::byteswap(value);
}

/**
 * Retrieve data of a certain bit length from an arbitrary position in a byte stream.
 * @param buff input buff.
 * @param start_bit_pos start from 0.
 * @param need_bit_count The required bit length has a range of [1, 64].
 * @param little_endian_byte_order true for Little-endian byte order system.
 */
inline unsigned long long get_bit_value(
  const void* buff,
  const int   start_bit_pos,
  const int   need_bit_count,
  const bool  little_endian_byte_order = true)
{
  auto input = static_cast<const unsigned char*>(buff);

  unsigned long long out = 0;
  if (need_bit_count > 64 || need_bit_count <= 0 || start_bit_pos < 0)
  {
    assert(false);
    return out;
  }

  int           in_first_byte_len = (std::min)(8 - start_bit_pos % 8, need_bit_count);
  unsigned char in_first_byte_value =
    static_cast<unsigned char>(input[start_bit_pos / 8] << (start_bit_pos % 8)) >>
    (8 - in_first_byte_len);

  int left_len = (std::max)(need_bit_count - in_first_byte_len, 0);

  if (left_len)
  {
    memcpy(
      &out,
      input + start_bit_pos / 8 + 1,
      (left_len % 8) ? (left_len / 8 + 1) : (left_len / 8));

    if (little_endian_byte_order)
    {
      out = nly::byteswap(out);
    }

    out >>= (sizeof(unsigned long long) * 8 - left_len);
    out = (static_cast<unsigned long long>(in_first_byte_value) << left_len) | out;
  }
  else
  {
    out = in_first_byte_value;
  }

  return out;
}

std::string hex_to_str(
  const void*                      input,
  const size_t                     input_byte,
  const std::optional<std::string> seg = " ",
  const bool                       capital = true)
{
  std::string out;
  out.reserve(input_byte * (seg ? (2 + seg->size()) : 2));

  std::string tmp = "{:02X}";
  if (!capital)
  {
    tmp = "{:02x}";
  }
  if (seg)
  {
    tmp += *seg;
  }

  fmt::format_string<char> format(tmp);

  auto input_start = static_cast<const unsigned char*>(input);
  auto input_end = input_start + input_byte;
  while (input_start != input_end)
  {
    out += fmt::format(format, *input_start++);
  }

  if (!out.empty() && seg)
  {
    assert(out.size() > seg->size());
    out.erase(out.end() - seg->size(), out.end());
  }

  return out;
}

/*
Note:
  1. All space will be ignore.
  2. The input must be a hexadecimal string. If it does not meet this requirement, the output will
     not be guaranteed to be correct.

input            output
""         ->    []
"   "      ->    []
"f"        ->    [0x0f]
" f  "     ->    [0x0f]
"fa"       ->    [0xfa]
"  f a "   ->    [0xfa]
"fab"      ->    [0x0f, 0xab]
"fabc"     ->    [0xfa, 0xbc]
*/
void str_to_hex(const std::string& input, std::vector<unsigned char>& output)
{
  output.clear();
  if (input.empty())
  {
    return;
  }

  std::string input_copy;
  input_copy.reserve(input.size());

  std::copy_if(
    std::begin(input),
    std::end(input),
    std::back_inserter(input_copy),
    [](char value) { return value != ' '; });
  output.reserve(input_copy.size() / 2);

  char buff[3] = {};
  auto it = input_copy.begin();
  auto it_end = input_copy.end();

  if (input_copy.size() % 2)
  {
    buff[0] = *it++;
    output.emplace_back(static_cast<unsigned char>(strtol(buff, nullptr, 16)));
  }

  while (it != it_end)
  {
    buff[0] = *it++;
    buff[1] = *it++;
    assert(it <= it_end);
    output.emplace_back(static_cast<unsigned char>(strtol(buff, nullptr, 16)));
  }
}

} // namespace nly

#endif