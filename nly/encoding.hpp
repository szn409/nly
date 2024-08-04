#ifndef NLY_ENCODING
#define NLY_ENCODING

#include "boost/locale/encoding.hpp"
#include <optional>
#include <string>

namespace nly
{
inline std::optional<std::string> between_non_utf(
  const void*        start,
  const void*        end,
  const std::string& from_encoding,
  const std::string& to_encoding)
{
  try
  {
    return boost::locale::conv::between(
      static_cast<const char*>(start),
      static_cast<const char*>(end),
      to_encoding,
      from_encoding,
      boost::locale::conv::stop);
  }
  catch (...)
  {
    return std::optional<std::string>();
  }
}

inline std::optional<std::string> between_non_utf(
  const void*        input,
  const size_t       input_bytes,
  const std::string& from_encoding,
  const std::string& to_encoding)
{
  return between_non_utf(
    input,
    static_cast<const char*>(input) + input_bytes,
    from_encoding,
    to_encoding);
}

// 1.If the t_char type is char, the output will be in UTF-8 encoding.
// 2.If the t_char type is wchar_t, the encoding of the output, whether it is UTF-16 or UTF-32,
// depends on the current system and the compiler's definition of wchar_t.
//  A.On Windows, it is UTF-16.
//  B.On Linux, it is default to UTF-32.
template<typename t_char>
std::optional<std::basic_string<t_char>> to_utf(
  const void*        start,
  const void*        end,
  const std::string& from_encoding)
{
  try
  {
    return boost::locale::conv::to_utf<t_char>(
      static_cast<const char*>(start),
      static_cast<const char*>(end),
      from_encoding,
      boost::locale::conv::stop);
  }
  catch (...)
  {
    return std::optional<std::basic_string<t_char>>();
  }
}

template<typename t_char>
std::optional<std::basic_string<t_char>> to_utf(
  const void*        input,
  const size_t       input_bytes,
  const std::string& from_encoding)
{
  return to_utf<t_char>(input, static_cast<const char*>(input) + input_bytes, from_encoding);
}

// The meaning of t_char refers to to_utf.
template<typename t_char>
std::optional<std::string> from_utf(
  const t_char*      start,
  const t_char*      end,
  const std::string& to_encoding)
{
  try
  {
    return boost::locale::conv::from_utf<t_char>(
      start,
      end,
      to_encoding,
      boost::locale::conv::stop);
  }
  catch (...)
  {
    return std::optional<std::string>();
  }
}

// t_char_out and t_char_in:
//  char: UTF-8
//  char_16: UTF-16
//  char_32: UTF-32
template<typename t_char_out, typename t_char_in>
std::optional<std::basic_string<t_char_out>> utf_to_utf(
  const t_char_in* start,
  const t_char_in* end)
{
  try
  {
    return boost::locale::conv::utf_to_utf<t_char_out>(start, end, boost::locale::conv::stop);
  }
  catch (...)
  {
    return std::optional<std::basic_string<t_char_out>>();
  }
}

} // namespace nly

#endif
