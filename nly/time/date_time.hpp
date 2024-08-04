#ifndef NLY_DATE_TIME
#define NLY_DATE_TIME

// If this macro is defined, then enable nanosecond precision, otherwise use microsecond precision.
// #define BOOST_DATE_TIME_POSIX_TIME_STD_CONFIG

#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

namespace nly
{
// Subclasses of date_duration: years, months, weeks can trigger the end-of-month trap, so use them
// with caution.
typedef boost::gregorian::years  years;
typedef boost::gregorian::months months;
typedef boost::gregorian::days   days; // days can be negative.

typedef boost::gregorian::year_iterator  year_iterator;
typedef boost::gregorian::month_iterator month_iterator;
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
typedef boost::posix_time::ptime         ptime;

} // namespace nly

// date convenient functions.
namespace nly
{
// For invalid input, it will throw std::out_of_range exception.
inline date make_date(int year, int month, int day)
{
  return date(year, month, day);
}

// 9999-12-31
inline date max_date()
{
  return date(boost::date_time::max_date_time);
}

// 1400-01-01
inline date min_date()
{
  return date(boost::date_time::min_date_time);
}

inline date local_day()
{
  return boost::gregorian::day_clock::local_day();
}
inline date universal_day()
{
  return boost::gregorian::day_clock::universal_day();
}

// for normal date, return string format is "XXXX-XX-XX"
inline std::string to_string(const date& d)
{
  return boost::gregorian::to_iso_extended_string(d);
}

// for normal date, return string format is "YYYYMMDD"
inline std::string to_undelimited_string(const date& d)
{
  return boost::gregorian::to_iso_string(d);
}

// support separators: { ',', '-', '.', ' ', '/' };
inline date date_from_string(const std::string& str)
{
  return boost::gregorian::from_string(str);
}

// support format: "YYYYMMDD"
inline date date_from_undelimited_string(const std::string& str)
{
  return boost::gregorian::from_undelimited_string(str);
}

inline date date_from_tm(const tm& t)
{
  return boost::gregorian::date_from_tm(t);
}
inline tm date_to_tm(const date& d)
{
  return boost::gregorian::to_tm(d);
}

} // namespace nly

// time_duration convenient functions.
namespace nly
{
// The unit of fractional_second depends on the precision, and will automatically round up.
inline time_duration make_time_duration(
  long long hour,
  long long minute,
  long long second,
  long long fractional_second = 0)
{
  return time_duration(hour, minute, second, fractional_second);
}

// support separators: { '-' ':' ',' '.' }
inline time_duration time_duration_from_string(const std::string& str)
{
  return boost::posix_time::duration_from_string(str);
}

// for normal time_duration, return string format is "XX:XX:XX" or "XX:XX:XX.XXXXXX" or
// "XX:XX:XX.XXXXXXXXX"
inline std::string to_string(const time_duration& t)
{
  return boost::posix_time::to_simple_string(t);
}

// for normal time_duration, return string format is "XXXXXX" or "XXXXXX.XXXXXX" or
// "XXXXXX.XXXXXXXXX"
inline std::string to_undelimited_string(const time_duration& t)
{
  return boost::posix_time::to_iso_string(t);
}

inline tm to_tm(const time_duration& t)
{
  return boost::posix_time::to_tm(t);
}

} // namespace nly

// time_duration convenient functions.
namespace nly
{
inline ptime make_ptime(const date& d)
{
  return boost::posix_time::ptime(d);
}

inline ptime make_ptime(const date& d, const time_duration& t)
{
  return boost::posix_time::ptime(d, t);
}

inline ptime make_ptime(
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

inline ptime ptime_from_string(std::string str)
{
  std::replace(str.begin(), str.end(), ' ', 'T');
  return boost::posix_time::from_iso_extended_string(str);
}

// for normal ptime, return string format is "XXXX-XX-XX XX:XX:XX" or "XXXX-XX-XX XX:XX:XX.XXXXXX"
// or "XXXX-XX-XX XX:XX:XX.XXXXXXXXX"
inline std::string to_string(const ptime& t)
{
  auto str = boost::posix_time::to_iso_extended_string(t);
  if (str.size() >= 10 && str[10] == 'T')
  {
    str[10] = ' ';
  }
  return str;
}

inline ptime local_time()
{
  return boost::posix_time::second_clock::local_time();
}
inline ptime local_micro_time()
{
  return boost::posix_time::microsec_clock::local_time();
}
inline ptime universal_time()
{
  return boost::posix_time::second_clock::universal_time();
}
inline ptime universal_micro_time()
{
  return boost::posix_time::microsec_clock::universal_time();
}

inline ptime ptime_from_tm(const tm& t)
{
  return boost::posix_time::ptime_from_tm(t);
}
inline tm to_tm(const ptime& t)
{
  return boost::posix_time::to_tm(t);
}

} // namespace nly

#endif