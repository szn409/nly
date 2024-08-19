#ifndef NLY_BIT
#define NLY_BIT

#include "boost/core/bit.hpp"

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

} // namespace nly

#endif