#ifndef NLY_BOOST_HELPER_SERIALIZATION
#define NLY_BOOST_HELPER_SERIALIZATION
#include "boost/archive/binary_iarchive.hpp"
#include "boost/archive/binary_oarchive.hpp"
#include "boost/archive/text_iarchive.hpp"
#include "boost/archive/text_oarchive.hpp"
#include "asio_helper/streambuf.hpp"
#include <string>
#include <sstream>
#include <optional>

namespace nly
{

class serialization
{
public:
  // 注意: buff 可以预先使用 prepare_output 来加快速度(避免内存分配)
  template<typename T>
  static void to_binary(const T& t, nly::streambuf& buff)
  {
    buff.consume_input(static_cast<size_t>(-1));
    boost::archive::binary_oarchive oa(buff.get());
    oa & t;
  }

  template<typename T>
  static bool from_binary(T& t, nly::streambuf& buff)
  {
    try
    {
      boost::archive::binary_iarchive ia(buff.get());
      ia & t;
    }
    catch (...)
    {
      return false;
    }

    return true;
  }

  // 实测不支持 2GB 以上的数据量
  template<typename T>
  static void to_text(const T& t, std::string& str)
  {
    std::ostringstream oss;

    boost::archive::text_oarchive oa(oss);
    oa & t;

    str = oss.str();
  }

  // 实测不支持 2GB 以上的数据量
  template<typename T>
  static std::string to_text(const T& t)
  {
    std::string str;
    to_text(t, str);
    return std::move(str);
  }

  template<typename T>
  static bool from_text(T& t, const std::string& data)
  {
    try
    {
      std::istringstream            iss(data);
      boost::archive::text_iarchive ia(iss);
      ia & t;
    }
    catch (...)
    {
      return false;
    }

    return true;
  }

  template<typename T>
  static std::optional<T> from_text(const std::string& data)
  {
    T t;
    return from_text(t, data) ? t : std::optional<T>{};
  }
};

} // namespace nly

#endif
