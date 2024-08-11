#include "gtest/gtest.h"
#include "nly/random.hpp"
#include <map>
#include <iomanip>

TEST(Random, All)
{
  auto e = nly::create_random_device();

  auto ui = nly::create_uniform_int_distribution(1, 100);
  EXPECT_TRUE(ui.min() == 1 && ui.max() == 100);

  auto ud = nly::create_uniform_real_distribution(1.0, 100.0);
  EXPECT_DOUBLE_EQ(ud.min(), 1.0);
  EXPECT_DOUBLE_EQ(ud.max(), 100.0);

  auto ub = nly::create_bernoulli_distribution(0.8);
  int  count = 0;
  for (int i = 0; i < 10000; ++i)
  {
    if (ub(e))
    {
      ++count;
    }
  }
  EXPECT_TRUE(count >= 7000);

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
}
