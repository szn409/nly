#ifndef NLY_STL_HELPER_RANDOM
#define NLY_STL_HELPER_RANDOM
#include <random>
#include <cassert>

namespace nly
{

class random
{
public:
  /*
    std::random_device
    1. 是一个均匀分布的整数随机生成器, 用于生成非确定性的随机数
    2. 若实现中没有可用的非确定源(比如硬件设备), 则每个 std::random_device 可能产生相同的数字序列
    3. std::default_random_engine 和 std::random_device 都可以与 std::uniform_int_distribution
    等类一起使用
  */
  static std::random_device create_random_device()
  {
    return std::random_device();
  }

  template<typename T = int>
  static std::uniform_int_distribution<T> create_uniform_int_distribution(T min_value, T max_value)
  {
    assert(min_value < max_value);
    return std::uniform_int_distribution(min_value, max_value);
  }

  template<typename T = double>
  static std::uniform_real_distribution<T> create_uniform_real_distribution(
    T min_value,
    T max_value)
  {
    assert(min_value < max_value);
    return std::uniform_real_distribution(min_value, max_value);
  }

  // p: 生成值为 true 的概率, 范围: [0, 1]
  static std::bernoulli_distribution create_bernoulli_distribution(double p)
  {
    return std::bernoulli_distribution(p);
  }

  /*
   * 生成服从正态分布的随机数
   * avg: 均值
   * standard_deviation: 标准差
   */
  template<typename T = double>
  static std::normal_distribution<T> create_normal_distribution(T avg, T standard_deviation)
  {
    return std::normal_distribution(avg, standard_deviation);
  }
};

} // namespace nly

#endif
