#ifndef NLY_BOOST_HELPER_DATE_TIME
#define NLY_BOOST_HELPER_DATE_TIME

// 若定义了该宏, 则开启纳秒精度, 否则使用微秒精度
// #define BOOST_DATE_TIME_POSIX_TIME_STD_CONFIG

#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

namespace nly
{
class boost_date_time
{
public:
  // 对于 years, months, weeks 使用时注意月末陷阱
  typedef boost::gregorian::years  years;
  typedef boost::gregorian::months months;
  typedef boost::gregorian::weeks  weeks;
  typedef boost::gregorian::days   days;

  typedef boost::gregorian::year_iterator  year_iterator;
  typedef boost::gregorian::month_iterator month_iterator;
  typedef boost::gregorian::week_iterator  week_iterator;
  typedef boost::gregorian::day_iterator   day_iterator;

  typedef boost::gregorian::date        date;
  typedef boost::gregorian::date_period date_period;

  typedef boost::posix_time::time_duration time_duration;
  typedef boost::posix_time::hours         hours;
  typedef boost::posix_time::minutes       minutes;
  typedef boost::posix_time::seconds       seconds;
  typedef boost::posix_time::milliseconds  milliseconds;
  typedef boost::posix_time::microseconds  microseconds;
#if defined(BOOST_DATE_TIME_HAS_NANOSECONDS)
  typedef boost::posix_time::nanoseconds nanoseconds;
#endif

  typedef boost::posix_time::time_iterator time_iterator;
  typedef boost::posix_time::time_period   time_period;

  // 在需要记录 time point 且字符串化时, ptime 比 std::chrono::system_clock::time_point 更好
  // ptime 自身占用 8 or 16 个字节, 并且支持使用 boost 序列化库进行序列化, 且转换字符串非常方便
  // std::chrono::system_clock::time_point 转字符串需要 std::localtime 系列函数, 不安全或不方便
  typedef boost::posix_time::ptime ptime;

public:
  // 对于无效输入, 会抛出 std::out_of_range 异常
  static date make_date(int year, int month, int day)
  {
    return date(year, month, day);
  }

  // 9999-12-31
  static date max_date()
  {
    return date(boost::date_time::max_date_time);
  }

  // 1400-01-01
  static date min_date()
  {
    return date(boost::date_time::min_date_time);
  }

  static date local_day()
  {
    return boost::gregorian::day_clock::local_day();
  }

  static date universal_day()
  {
    return boost::gregorian::day_clock::universal_day();
  }

  // 对于普通的 date, 返回的格式是: "XXXX-XX-XX"
  static std::string to_string(const date& d)
  {
    return boost::gregorian::to_iso_extended_string(d);
  }

  // 对于普通的 date, 返回的格式是: "YYYYMMDD"
  static std::string to_undelimited_string(const date& d)
  {
    return boost::gregorian::to_iso_string(d);
  }

  // 支持的分隔符: { ',', '-', '.', ' ', '/' };
  static date date_from_string(const std::string& str)
  {
    return boost::gregorian::from_string(str);
  }

  // 支持的格式: "YYYYMMDD"
  static date date_from_undelimited_string(const std::string& str)
  {
    return boost::gregorian::from_undelimited_string(str);
  }

  static date date_from_tm(const tm& t)
  {
    return boost::gregorian::date_from_tm(t);
  }

  static tm date_to_tm(const date& d)
  {
    return boost::gregorian::to_tm(d);
  }

public:
  // fractional_second 的单位取决于精度, 并且会自动向上取整
  // 精度: 见上文 BOOST_DATE_TIME_POSIX_TIME_STD_CONFIG 的说明
  static time_duration make_time_duration(
    long long hour,
    long long minute,
    long long second,
    long long fractional_second = 0)
  {
    return time_duration(hour, minute, second, fractional_second);
  }

  // 支持的分隔符: { '-' ':' ',' '.' }
  static time_duration time_duration_from_string(const std::string& str)
  {
    return boost::posix_time::duration_from_string(str);
  }

  // 对于普通的 time_duration, 返回格式: "XX:XX:XX" or "XX:XX:XX.XXXXXX" or "XX:XX:XX.XXXXXXXXX"
  static std::string to_string(const time_duration& t)
  {
    return boost::posix_time::to_simple_string(t);
  }

  // 对于普通的 time_duration, 返回格式: "XXXXXX" or "XXXXXX.XXXXXX" or "XXXXXX.XXXXXXXXX"
  static std::string to_undelimited_string(const time_duration& t)
  {
    return boost::posix_time::to_iso_string(t);
  }

  static tm to_tm(const time_duration& t)
  {
    return boost::posix_time::to_tm(t);
  }

public:
  static ptime make_ptime(const date& d)
  {
    return boost::posix_time::ptime(d);
  }

  static ptime make_ptime(const date& d, const time_duration& t)
  {
    return boost::posix_time::ptime(d, t);
  }

  static ptime make_ptime(
    int       year,
    int       month,
    int       day,
    long long hour,
    long long minute,
    long long second,
    long long fractional_second = 0)
  {
    auto date = make_date(year, month, day);
    auto time = make_time_duration(hour, minute, second, fractional_second);
    return boost::posix_time::ptime(date, time);
  }

  static ptime ptime_from_string(std::string str)
  {
    std::replace(str.begin(), str.end(), ' ', 'T');
    return boost::posix_time::from_iso_extended_string(str);
  }

  // 对于普通的 ptime, 返回格式为: "XXXX-XX-XX XX:XX:XX" or "XXXX-XX-XX XX:XX:XX.XXXXXX" or
  // "XXXX-XX-XX XX:XX:XX.XXXXXXXXX"
  static std::string to_string(const ptime& t)
  {
    auto str = boost::posix_time::to_iso_extended_string(t);
    if (str.size() >= 10 && str[10] == 'T')
    {
      str[10] = ' ';
    }
    return str;
  }

  static ptime local_time()
  {
    return boost::posix_time::second_clock::local_time();
  }

  static ptime local_micro_time()
  {
    return boost::posix_time::microsec_clock::local_time();
  }

  static ptime universal_time()
  {
    return boost::posix_time::second_clock::universal_time();
  }

  static ptime universal_micro_time()
  {
    return boost::posix_time::microsec_clock::universal_time();
  }

  static ptime ptime_from_tm(const tm& t)
  {
    return boost::posix_time::ptime_from_tm(t);
  }

  static tm to_tm(const ptime& t)
  {
    return boost::posix_time::to_tm(t);
  }
};

} // namespace nly

#endif