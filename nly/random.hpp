#ifndef NLY_RANDOM
#define NLY_RANDOM
#include <random>
#include <cassert>

namespace nly
{

/*
std::random_device is a uniformly-distributed integer random number generator that produces
non-deterministic random numbers.
if a non-deterministic source (e.g. a hardware device) is not available to the implementation. In
this case each std::random_device object may generate the same number sequence.

Note: Both std::default_random_engine and std::random_device can be used with classes like
std::uniform_int_distribution.
*/
inline std::random_device create_random_device()
{
  return std::random_device();
}

template<typename T = int>
inline std::uniform_int_distribution<T> create_uniform_int_distribution(T min_value, T max_value)
{
  assert(min_value < max_value);
  return std::uniform_int_distribution(min_value, max_value);
}

template<typename T = double>
inline std::uniform_real_distribution<T> create_uniform_real_distribution(T min_value, T max_value)
{
  assert(min_value < max_value);
  return std::uniform_real_distribution(min_value, max_value);
}

// p: probability of generating true.
// the range of p is [0, 1].
inline std::bernoulli_distribution create_bernoulli_distribution(double p)
{
  return std::bernoulli_distribution(p);
}

/*
  auto               un = nly::create_normal_distribution(5.0, 2.0);
  std::map<int, int> hist{};
  for (int i = 0; i < 10000; ++i)
  {
    ++hist[static_cast<int>(std::round(un(e)))];
  }
  for (auto [x, y] : hist)
  {
    std::cout << std::setw(2) << x << ' ' << std::string(y / 50, '*') << '\n';
  }
  // -3
  // -2
  // -1
  //  0 *
  //  1 *****
  //  2 *************
  //  3 ************************
  //  4 ************************************
  //  5 **************************************
  //  6 ***********************************
  //  7 ***********************
  //  8 *************
  //  9 *****
  // 10 *
  // 11
  // 12
  // 13
*/
template<typename T = double>
inline std::normal_distribution<T> create_normal_distribution(T avg, T standard_deviation)
{
  return std::normal_distribution(avg, standard_deviation);
}

} // namespace nly

#endif
