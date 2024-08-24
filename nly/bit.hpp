#ifndef NLY_BIT
#define NLY_BIT

#include "boost/core/bit.hpp"
#include "fmt/format.h"
#include <vector>
#include <cassert>
#include <optional>
#include <iterator>
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

inline std::string hex_to_str(
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
inline void str_to_hex(const std::string& input, std::vector<unsigned char>& output)
{
  output.clear();
  output.reserve(input.size() / 2);

  auto it = input.rbegin();
  auto it_end = input.rend();

  auto next = [&it, &it_end]() -> bool
  {
    while (it != it_end && *it == ' ')
    {
      ++it;
    }

    return it != it_end;
  };

  char buff[3] = {};

  while (true)
  {
    if (!next())
    {
      break;
    }
    buff[1] = *it++;

    if (!next())
    {
      output.emplace_back(static_cast<unsigned char>(strtol(buff + 1, nullptr, 16)));
      break;
    }
    buff[0] = *it++;
    output.emplace_back(static_cast<unsigned char>(strtol(buff, nullptr, 16)));
  }

  std::reverse(output.begin(), output.end());
  return;
}

// Each time, take 10 consecutive bits from the input, form a 16-bit value.
// The number of input bytes must be a multiple of 5.
// The number of valid data bytes in the output is input_byte / 5 * 4.
inline void from_10bit_to_16bit(const void* input, const size_t input_byte, unsigned short* output)
{
  assert(!(input_byte % 5));

  auto input_begin = static_cast<const unsigned char*>(input);
  auto input_end = input_begin + input_byte;
  int  index = 0;

  unsigned char tem_0 = 0;
  unsigned char tem_1 = 0;
  unsigned char tem_2 = 0;
  unsigned char tem_3 = 0;
  unsigned char tem_4 = 0;

  while (input_begin != input_end)
  {
    tem_0 = *input_begin++;
    tem_1 = *input_begin++;
    tem_2 = *input_begin++;
    tem_3 = *input_begin++;
    tem_4 = *input_begin++;

    output[index++] = tem_0 << 2 | tem_1 >> 6;
    output[index++] = (tem_1 & 0x3f) << 4 | tem_2 >> 4;
    output[index++] = (tem_2 & 0xf) << 6 | tem_3 >> 2;
    output[index++] = (tem_3 & 0x3) << 8 | tem_4;
  }
}

// Each time, take 12 consecutive bits from the input, form a 16-bit value.
// The number of input bytes must be a multiple of 3.
// The number of valid data bytes in the output is input_byte / 3 * 2.
inline void from_12bit_to_16bit(const void* input, const size_t input_byte, unsigned short* output)
{
  assert(!(input_byte % 3));

  auto input_start = static_cast<const unsigned char*>(input);
  auto input_end = input_start + input_byte;
  int  index = 0;

  unsigned char tem_0 = 0;
  unsigned char tem_1 = 0;
  unsigned char tem_2 = 0;

  while (input_start != input_end)
  {
    tem_0 = *input_start++;
    tem_1 = *input_start++;
    tem_2 = *input_start++;

    output[index++] = tem_0 << 4 | tem_1 >> 4;
    output[index++] = (tem_1 & 0xF) << 8 | tem_2;
  }
}

// For each element in the input, take 8 consecutive bits, store them in the output.
// mode: 0 means to take the lowest 8 bits of each element, 1 means to take the second lowest 8
// bits, and so on.
// mode: [0, 8].
inline void from_16bit_to_8bit(
  const unsigned short* input,
  const int             input_item_count,
  unsigned char*        output,
  const int             mode)
{
  assert(mode >= 0 && mode <= 8);
  auto input_begin = input;
  auto input_end = input + input_item_count;
  int  index = 0;

  while (input_begin != input_end)
  {
    output[index++] = static_cast<unsigned char>(((*input_begin++) >> mode) & 0xFF);
  }
}

} // namespace nly

#endif