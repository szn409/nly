#ifndef NLY_MATH
#define NLY_MATH
#include <cmath>

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
};

} // namespace nly

#endif