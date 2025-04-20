#ifndef NLY_BOOST_HELPER_SERIALIZATION
#define NLY_BOOST_HELPER_SERIALIZATION
#include "boost/archive/binary_iarchive.hpp"
#include "boost/archive/binary_oarchive.hpp"
#include "boost/archive/text_iarchive.hpp"
#include "boost/archive/text_oarchive.hpp"
#include <string>
#include <sstream>
#include <optional>

namespace nly
{

class serialization
{
public:
  template<typename T>
  static void to_data(const T& t, std::string& str, bool use_binary = true)
  {
    std::ostringstream oss;

    if (use_binary)
    {
      boost::archive::binary_oarchive oa(oss);
      oa & t;
    }
    else
    {
      boost::archive::text_oarchive oa(oss);
      oa & t;
    }

    str = oss.str();
  }

  template<typename T>
  static std::string to_data(const T& t, bool use_binary = true)
  {
    std::string str;
    to_data(t, str, use_binary);
    return std::move(str);
  }

  template<typename T>
  static bool from_data(T& t, const std::string& data, bool use_binary = true)
  {
    try
    {
      std::istringstream iss(data);

      if (use_binary)
      {
        boost::archive::binary_iarchive ia(iss);
        ia & t;
      }
      else
      {
        boost::archive::text_iarchive ia(iss);
        ia & t;
      }
    }
    catch (...)
    {
      return false;
    }

    return true;
  }

  template<typename T>
  static std::optional<T> from_data(const std::string& data, bool use_binary = true)
  {
    T t;
    return from_data(t, data, use_binary) ? t : std::optional<T>{};
  }
};

} // namespace nly

#endif
