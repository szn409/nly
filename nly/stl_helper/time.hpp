#ifndef NLY_STL_HELPER_TIME
#define NLY_STL_HELPER_TIME
#include <chrono>

namespace nly
{

// 提供了简单的计时功能
class time_count
{
public:
  using time_point = std::chrono::steady_clock::time_point;

public:
  // 获取当前时间
  static time_point now()
  {
    return std::chrono::steady_clock::now();
  }

  // 返回值: t1 - t0
  // 单位: 秒
  static double time_diff(const time_point& t0, const time_point& t1 = time_count::now())
  {
    return std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() / 1000000.0;
  }

  // 返回值: t1 - t0
  // 单位: 毫秒
  static long long time_diff_ms(const time_point& t0, const time_point& t1 = time_count::now())
  {
    return std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
  }

  // 返回值: t1 - t0
  // 单位: 微秒
  static long long time_diff_us(const time_point& t0, const time_point& t1 = time_count::now())
  {
    return std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
  }
};

} // namespace nly

#endif