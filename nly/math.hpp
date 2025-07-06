#ifndef NLY_MATH
#define NLY_MATH
#include <cmath>

// 拷贝自 msvc 的 corecrt_math_defines.h
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace nly
{

class math
{
public:
  // 判断两个浮点数之差, 是否 <= 指定值
  template<typename T, typename U>
  static bool float_equal(T t, U u, double allow_diff = 1e-10)
  {
    return std::abs(t - u) <= allow_diff;
  }

  // 角度转弧度
  static double to_rad(double angle)
  {
    // 0.017453292519943295: M_PI / 180.0
    return angle * 0.017453292519943295;
  }

  // 弧度转角度
  static double to_angle(double rad)
  {
    // 57.29577951308232: 180.0 / M_PI
    return rad * 57.29577951308232;
  }
};

} // namespace nly

#endif