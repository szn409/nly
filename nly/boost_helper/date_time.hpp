#ifndef NLY_BOOST_HELPER_DATE_TIME
#define NLY_BOOST_HELPER_DATE_TIME

// 若定义了该宏, 则开启纳秒精度, 否则使用微秒精度
// #define BOOST_DATE_TIME_POSIX_TIME_STD_CONFIG

#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "nly/boost_helper/string.hpp"
#include "fmt/format.h"
#include <array>
#include <optional>

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
  static date make_date(unsigned short year, unsigned short month, unsigned short day)
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
    unsigned short year,
    unsigned short month,
    unsigned short day,
    long long      hour,
    long long      minute,
    long long      second,
    long long      fractional_second = 0)
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

public:
  /*
  功能: 获取当前的时间字符串, 可自定义格式(默认格式为: XXXX-XX-XX XX-XX-XX)
  形参:
    num_fractional: 秒后的小数部分的位数, 0 标识没有小数部分
    seg_ymd: 年月日之间的分隔符
    seg_join: 用于连接年月日和时分秒
    seg_hms: 时分秒之间的分隔符
    seg_fractional: 秒后的小数部分与秒之间的分隔符, 若 num_fractional 为 0, 则结果不含此部分
    use_local_time:
      true: 使用本地时区
      false: 使用 universal 时区
  注意: 若传入的分隔符无效, 则标识不使用分隔符
  */
  static std::string get_now_time_str(
    int                        num_fractional = 0,
    std::optional<std::string> seg_ymd = "-",
    std::optional<std::string> seg_join = " ",
    std::optional<std::string> seg_hms = "-",
    std::optional<std::string> seg_fractional = ".",
    bool                       use_local_time = true)
  {
    boost_date_time::ptime now_time;

    if (use_local_time)
    {
      if (0 == num_fractional)
      {
        now_time = boost_date_time::local_time();
      }
      else
      {
        now_time = boost_date_time::local_micro_time();
      }
    }
    else
    {
      if (0 == num_fractional)
      {
        now_time = boost_date_time::universal_time();
      }
      else
      {
        now_time = boost_date_time::universal_micro_time();
      }
    }

    std::array<std::string, 3> tmp;
    fmt::format_string<char>   fmt_str("{:02}");

    const auto date = now_time.date();
    tmp[0] = std::to_string(static_cast<unsigned short>(date.year()));
    tmp[1] = fmt::format(fmt_str, date.month().as_number());
    tmp[2] = fmt::format(fmt_str, date.day().as_number());
    const std::string date_part = nly::join(tmp, seg_ymd ? *seg_ymd : "");

    const auto time_of_day = now_time.time_of_day();
    tmp[0] = fmt::format(fmt_str, time_of_day.hours());
    tmp[1] = fmt::format(fmt_str, time_of_day.minutes());
    tmp[2] = fmt::format(fmt_str, time_of_day.seconds());
    const std::string time_part = nly::join(tmp, seg_hms ? *seg_hms : "");

    auto output = date_part + (seg_join ? *seg_join : "") + time_part;

    if (num_fractional)
    {
      std::string fractional_part =
        fmt::format("{:0<{}}", time_of_day.fractional_seconds(), num_fractional);
      if (fractional_part.size() > num_fractional)
      {
        fractional_part.resize(num_fractional);
      }

      output += ((seg_fractional ? *seg_fractional : "") + fractional_part);
    }

    return output;
  }
};

} // namespace nly

#endif