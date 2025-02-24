#ifndef NLY_BOOST_HELPER_ENCODING
#define NLY_BOOST_HELPER_ENCODING

#include "boost/locale/encoding.hpp"
#include <optional>
#include <string>

namespace nly
{

class encoding
{
public:
  // 执行非 utf 之间的字符集转换
  static std::optional<std::string> between_non_utf(
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

  static std::optional<std::string> between_non_utf(
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

  /*
  执行非 utf 字符集, 转为 utf 字符集

  1. 若 t_char 的类型是 char, 则输出的编码是 UTF-8
  2. 若 t_char 的类型是 wchar_t, 则输出的编码是 UTF-16 或 UTF-32
      A. Windows 平台下: 是 UTF-16
      B. Linux 平台下: 默认是 UTF-32
  */
  template<typename t_char>
  static std::optional<std::basic_string<t_char>> to_utf(
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
  static std::optional<std::basic_string<t_char>> to_utf(
    const void*        input,
    const size_t       input_bytes,
    const std::string& from_encoding)
  {
    return to_utf<t_char>(input, static_cast<const char*>(input) + input_bytes, from_encoding);
  }

  // 执行 utf 字符集, 到非 utf 字符集的转换
  // 关于 t_char 的说明, 参考 to_utf
  template<typename t_char>
  static std::optional<std::string> from_utf(
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

  /*
  执行 utf 字符集之间的转换

  t_char_out 和 t_char_in:
    char: UTF-8
    char_16: UTF-16
    char_32: UTF-32
  */
  template<typename t_char_out, typename t_char_in>
  static std::optional<std::basic_string<t_char_out>> utf_to_utf(
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
};

} // namespace nly

#endif
