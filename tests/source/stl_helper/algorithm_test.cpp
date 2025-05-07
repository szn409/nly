#include "nly/stl_helper/algorithm.hpp"
#include "gtest/gtest.h"

TEST(Algorithm, ForEach)
{
  {
    std::vector<int> vec{ 0, 1, 2, 3 };
    auto             fun = [](int& value) { ++value; };

    nly::for_each(vec, fun);
    EXPECT_EQ(vec, (std::vector<int>{ 1, 2, 3, 4 }));

    nly::for_each(vec, fun, true);
    EXPECT_EQ(vec, (std::vector<int>{ 2, 3, 4, 5 }));
  }

  {
    int                    sum = 0;
    const std::vector<int> vec{ 0, 1, 2, 3 };
    auto                   fun = [&sum](const int& value) { sum += value; };
    nly::for_each(vec, fun);
    EXPECT_EQ(sum, 6);
  }
}

TEST(Algorithm, AllOf)
{
  std::vector<int> vec{ 0, 1, 2, 3 };

  EXPECT_TRUE(!nly::all_of(vec, [](int value) { return value > 1; }));
  EXPECT_TRUE(nly::all_of(vec, [](int value) { return value > -1; }));
  EXPECT_TRUE(!nly::all_of(vec, [](int value) { return value > 1; }, true));
  EXPECT_TRUE(nly::all_of(vec, [](int value) { return value > -1; }, true));

  EXPECT_TRUE(nly::all_of(std::vector<int>{}, [](int value) { return value > -1; }));
  EXPECT_TRUE(nly::all_of(std::vector<int>{}, [](int value) { return value > -1; }, true));
}

TEST(Algorithm, AnyOf)
{
  std::vector<int> vec{ 0, 1, 2, 3 };

  EXPECT_TRUE(!nly::any_of(vec, [](int value) { return value > 5; }));
  EXPECT_TRUE(nly::any_of(vec, [](int value) { return value > 2; }));
  EXPECT_TRUE(!nly::any_of(vec, [](int value) { return value > 5; }, true));
  EXPECT_TRUE(nly::any_of(vec, [](int value) { return value > 2; }, true));

  EXPECT_TRUE(!nly::any_of(std::vector<int>{}, [](int value) { return value > -1; }));
  EXPECT_TRUE(!nly::any_of(std::vector<int>{}, [](int value) { return value > -1; }, true));
}

TEST(Algorithm, NoneOf)
{
  std::vector<int> vec{ 0, 1, 2, 3 };

  EXPECT_TRUE(nly::none_of(vec, [](int value) { return value > 5; }));
  EXPECT_TRUE(!nly::none_of(vec, [](int value) { return value > 2; }));
  EXPECT_TRUE(nly::none_of(vec, [](int value) { return value > 5; }, true));
  EXPECT_TRUE(!nly::none_of(vec, [](int value) { return value > 2; }, true));

  EXPECT_TRUE(nly::none_of(std::vector<int>{}, [](int value) { return value > -1; }));
  EXPECT_TRUE(nly::none_of(std::vector<int>{}, [](int value) { return value > -1; }, true));
}

TEST(Algorithm, Find)
{
  {
    std::vector<int> vec{ 0, 1, 2, 3 };
    EXPECT_TRUE(*nly::find(vec, 2) == 2);
    EXPECT_TRUE(*nly::find(vec, 2, true) == 2);
    EXPECT_TRUE(nly::find(vec, 5) == vec.cend());
    EXPECT_TRUE(nly::find(vec, 5, true) == vec.cend());
  }

  {
    const std::vector<int> vec{ 0, 1, 2, 3 };
    EXPECT_TRUE(*nly::find(vec, 2) == 2);
  }
}