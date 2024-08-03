#ifndef NLY_TIME
#define NLY_TIME
#include <chrono>

namespace nly
{
using time_point = std::chrono::steady_clock::time_point;

inline time_point now()
{
  return std::chrono::steady_clock::now();
}

// return: t1 - t0
// unit: second
inline double time_diff(const time_point& t0, const time_point& t1 = nly::now())
{
  return std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() / 1000000.0;
}

// return: t1 - t0
// unit: millisecond
inline long long time_diff_ms(const time_point& t0, const time_point& t1 = nly::now())
{
  return std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
}

// return: t1 - t0
// unit: microsecond
inline long long time_diff_us(const time_point& t0, const time_point& t1 = nly::now())
{
  return std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
}

} // namespace nly

#endif