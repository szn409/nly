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

// T 必须是整数类型
template<typename T>
T byteswap(T value)
{
  return boost::core::byteswap(value);
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
注意:
  1. 所有的空格将被忽略
  2. 输入必须是 16 进制的字符串

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

/*
功能: 从字节流的任意 bit 位置, 获取指定 bit 长度的值
形参说明:
  buff: 输入缓冲区
  start_bit_pos: 数据获取时的 bit 位置, 从 0 开始计数
  need_bit_count: 所需的 bit 数, 有效范围: [1, 64]
  little_endian_byte_order:
    true: 适用于小端操作系统
    false: 适用于大端操作系统
 */
inline unsigned long long get_bit_value(
  const void* buff,
  const int   start_bit_pos,
  const int   need_bit_count,
  const bool  little_endian_byte_order = true)
{
  auto input = static_cast<const unsigned char*>(buff);

  unsigned long long out = 0;
  if (!buff || need_bit_count > 64 || need_bit_count <= 0 || start_bit_pos < 0)
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

/*
功能: 每次从 input 中, 获取连续的 10 bit, 存入到 output 中
注意:
  1. input_byte 必须是 5 的整数倍
  2. output 有效字节数: input_byte / 5 * 4.
*/
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

/*
功能: 每次从 input 中, 获取连续的 12 bit, 存入到 output 中
注意:
  1. input_byte 必须是 3 的整数倍
  2. output 有效字节数: input_byte / 3 * 2.
*/
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

/*
功能: 对于 input 中的每个元素, 获取连续的 8 bit, 存到 output 中
mode 说明:
  0 意味着取 input 每个元素的最低 8 bit
  1 意味着取 input 每个元素的倒数第二低 8 bit
  其他取值, 以此类推
  有效输入: [0, 8]
*/
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