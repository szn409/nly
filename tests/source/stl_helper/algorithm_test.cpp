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

TEST(Algorithm, FindIf)
{
  auto fun = [](int value) { return value >= 2; };

  {
    std::vector<int> vec{ 0, 1, 2, 3 };
    EXPECT_TRUE(*nly::find_if(vec, fun) == 2);
    EXPECT_TRUE(*nly::find_if(vec, fun, true) == 2);
  }

  {
    const std::vector<int> vec{ 0, 1, 2, 3 };
    EXPECT_TRUE(*nly::find_if(vec, fun) == 2);
  }
}

TEST(Algorithm, FindIfNot)
{
  auto fun = [](int value) { return value <= 2; };

  {
    std::vector<int> vec{ 0, 1, 2, 3 };
    EXPECT_TRUE(*nly::find_if_not(vec, fun) == 3);
    EXPECT_TRUE(*nly::find_if_not(vec, fun, true) == 3);
  }

  {
    const std::vector<int> vec{ 0, 1, 2, 3 };
    EXPECT_TRUE(*nly::find_if_not(vec, fun) == 3);
  }
}

TEST(Algorithm, FindEnd)
{
  {
    std::vector<int>       vec_0{ 0, 1, 2, 3, 4, 0, 1, 2 };
    const std::vector<int> vec_1{ 0, 1 };
    EXPECT_TRUE(nly::find_end(vec_0, vec_1) - vec_0.begin() == 5);
    EXPECT_TRUE(nly::find_end(vec_0, vec_1, true) - vec_0.begin() == 5);
  }

  {
    const std::vector<int> vec_0{ 0, 1, 2, 3, 4, 0, 1, 2 };
    std::vector<int>       vec_1{ 100, 200 };
    EXPECT_TRUE(nly::find_end(vec_0, vec_1) == vec_0.end());
    EXPECT_TRUE(nly::find_end(vec_0, vec_1, true) == vec_0.end());
  }

  {
    auto fun = [](int a, int b) { return a + 3 == b; };

    const std::vector<int> vec_0{ 0, 1, 2, 3, 4, 0, 1, 2 };
    const std::vector<int> vec_1{ 3, 4 };
    EXPECT_TRUE(nly::find_end(vec_0, vec_1, fun) - vec_0.begin() == 5);
    EXPECT_TRUE(nly::find_end(vec_0, vec_1, fun, true) - vec_0.begin() == 5);
  }

  {
    auto fun = [](int a, int b) { return a + 3 == b; };

    std::vector<int> vec_0{ 0, 1, 2, 3, 4, 0, 1, 2 };
    std::vector<int> vec_1{ 300, 400 };
    EXPECT_TRUE(nly::find_end(vec_0, vec_1, fun) == vec_0.end());
    EXPECT_TRUE(nly::find_end(vec_0, vec_1, fun, true) == vec_0.end());
  }
}

TEST(Algorithm, FindFirstOf)
{
  {
    std::vector<int>       vec_0{ 0, 1, 2, 3, 4 };
    const std::vector<int> vec_1{ 3, 5 };
    EXPECT_TRUE(nly::find_first_of(vec_0, vec_1) - vec_0.begin() == 3);
    EXPECT_TRUE(nly::find_first_of(vec_0, vec_1, true) - vec_0.begin() == 3);
  }

  {
    const std::vector<int> vec_0{ 0, 1, 2, 3, 4 };
    std::vector<int>       vec_1{ 100, 200 };
    EXPECT_TRUE(nly::find_first_of(vec_0, vec_1) == vec_0.end());
    EXPECT_TRUE(nly::find_first_of(vec_0, vec_1, true) == vec_0.end());
  }

  {
    auto fun = [](int a, int b) { return a + 3 == b; };

    const std::vector<int> vec_0{ 0, 1, 2, 3, 4 };
    const std::vector<int> vec_1{ 5, 6 };
    EXPECT_TRUE(nly::find_end(vec_0, vec_1, fun) - vec_0.begin() == 2);
    EXPECT_TRUE(nly::find_end(vec_0, vec_1, fun, true) - vec_0.begin() == 2);
  }

  {
    auto fun = [](int a, int b) { return a + 3 == b; };

    std::vector<int> vec_0{ 0, 1, 2, 3, 4 };
    std::vector<int> vec_1{ 300, 400 };
    EXPECT_TRUE(nly::find_end(vec_0, vec_1, fun) == vec_0.end());
    EXPECT_TRUE(nly::find_end(vec_0, vec_1, fun, true) == vec_0.end());
  }
}

TEST(Algorithm, AdjacentFind)
{
  {
    std::vector<int> vec{ 0, 1, 1, 2, 3, 3, 4 };
    EXPECT_TRUE(nly::adjacent_find(vec) - vec.begin() == 1);
    EXPECT_TRUE(nly::adjacent_find(vec, true) - vec.begin() == 1);
  }

  {
    const std::vector<int> vec{ 0, 1, 3, 2, 3, 4 };
    auto                   fun = [](int a, int b) { return a == 3 && b == 4; };
    EXPECT_TRUE(nly::adjacent_find(vec, fun) - vec.begin() == 4);
    EXPECT_TRUE(nly::adjacent_find(vec, fun, true) - vec.begin() == 4);
  }

  {
    std::vector<int> vec{ 0, 1, 2, 3, 4 };
    EXPECT_TRUE(nly::adjacent_find(vec) == vec.end());
    EXPECT_TRUE(nly::adjacent_find(vec, true) == vec.end());
  }
}

TEST(Algorithm, Count)
{
  {
    std::vector<int> vec{ 0, 1, 2, 3, 4, 4 };
    EXPECT_TRUE(nly::count(vec, 4) == 2);
    EXPECT_TRUE(nly::count(vec, 4, true) == 2);

    auto fun = [](int value) { return !(value % 2); };
    EXPECT_TRUE(nly::count_if(vec, fun) == 4);
    EXPECT_TRUE(nly::count_if(vec, fun, true) == 4);
  }

  {
    const std::vector<int> vec{ 0, 1, 2, 3, 4, 4 };
    EXPECT_TRUE(nly::count(vec, 10) == 0);
    EXPECT_TRUE(nly::count(vec, 10, true) == 0);

    auto fun = [](int value) { return value > 10; };
    EXPECT_TRUE(nly::count_if(vec, fun) == 0);
    EXPECT_TRUE(nly::count_if(vec, fun, true) == 0);
  }
}

TEST(Algorithm, Mismatch)
{
  std::vector<int> vec_base{ 0, 1, 2, 3, 4 };
  std::vector<int> vec_1{ 0, 1, 2, 3 };
  std::vector<int> vec_2{ 0, 1, 2, 4, 5 };
  std::vector<int> vec_3(vec_base);

  auto res = nly::mismatch(vec_base, vec_1);
  EXPECT_TRUE(res.first - vec_base.begin() == 4 && res.second == vec_1.end());

  res = nly::mismatch(vec_base, vec_2, true);
  EXPECT_TRUE(res.first - vec_base.begin() == 3 && res.second - vec_2.begin() == 3);

  res = nly::mismatch(vec_base, vec_3);
  EXPECT_TRUE(res.first == vec_base.end() && res.second == vec_3.end());

  res = nly::mismatch(vec_base, vec_2, [](int a, int b) { return a <= b; }, true);
  EXPECT_TRUE(res.first == vec_base.end() && res.second == vec_2.end());
}

TEST(Algorithm, Equal)
{
  std::vector<int> vec_base{ 0, 1, 2, 3, 4 };
  std::vector<int> vec_1{ 0, 1, 2, 3 };
  std::vector<int> vec_2{ 0, 1, 2, 4, 5 };
  std::vector<int> vec_3(vec_base);

  EXPECT_TRUE(!nly::equal(vec_base, vec_1));
  EXPECT_TRUE(!nly::equal(vec_base, vec_2, true));
  EXPECT_TRUE(!nly::equal(vec_base, vec_2));
  EXPECT_TRUE(!nly::equal(vec_base, vec_2, true));
  EXPECT_TRUE(nly::equal(vec_base, vec_3));
  EXPECT_TRUE(nly::equal(vec_base, vec_3, true));
  EXPECT_TRUE(nly::equal(vec_base, vec_2, [](int a, int b) { return std::abs(a - b) <= 2; }, true));
}

TEST(Algorithm, Search)
{
  int value_1[] = { 0, 1, 2, 3, 4, 5, 1, 2, 3, 6 };
  int value_2[] = { 1, 2, 3 };
  int value_3[] = { 10, 20, 30 };

  EXPECT_TRUE(nly::search(value_1, value_2) == value_1 + 1);
  EXPECT_TRUE(nly::search(value_1, value_2, true) == value_1 + 1);
  EXPECT_TRUE(nly::search(value_1, value_3, true) == std::end(value_1));

  const char str[] = "abc szn xyz szn";
  const char tmp[] = "szn";

  // 被搜索的是 's', 'z', 'n', '\0'
  EXPECT_TRUE(nly::search(str, tmp) == str + 12);
}

TEST(Algorithm, SearchN)
{
  int value[] = { 1, 2, 3, 1, 5, 7, 8, 8, 9 };

  auto fun = [](int a, int b) {};
  EXPECT_TRUE(nly::search_n(value, 2, 8, true) - value == 6);
  EXPECT_TRUE(nly::search_n(value, 3, 2, [](int a, int b) { return a >= b; }) - value == 4);
}

TEST(Algorithm, Copy)
{
  int              value[] = { 1, 2, 3, 4, 5 };
  std::vector<int> vec;

  auto reset = [&vec]()
  {
    vec.clear();
    vec.resize(5);
  };

  reset();
  nly::copy(value, vec.begin());
  EXPECT_TRUE(nly::equal(value, vec));

  reset();
  nly::copy(value, vec.begin(), true);
  EXPECT_TRUE(nly::equal(value, vec));

  auto fun = [](int value) { return value % 2; };

  reset();
  vec.erase(nly::copy_if(value, vec.begin(), fun), vec.end());
  EXPECT_TRUE(nly::equal(std::vector<int>{ 1, 3, 5 }, vec));

  reset();
  vec.erase(nly::copy_if(value, vec.begin(), fun, true), vec.end());
  EXPECT_TRUE(nly::equal(std::vector<int>{ 1, 3, 5 }, vec));

  std::vector<int> tmp{ 1, 2, 3, 4, 5 };
  std::copy_backward(tmp.begin(), tmp.begin() + 3, tmp.end());
  EXPECT_TRUE(nly::equal(tmp, std::vector<int>{ 1, 2, 1, 2, 3 }));

  std::vector<int> tmp1{ 1, 2, 3, 4, 5 };
  std::copy(tmp1.begin() + 3, tmp1.end(), tmp1.begin());
  EXPECT_TRUE(nly::equal(tmp1, std::vector<int>{ 4, 5, 3, 4, 5 }));
}

TEST(Algorithm, Move)
{
  std::vector<std::string> vec;

  auto get_base = []() { return std::vector<std::string>{ "12", "34", "567" }; };

  for (int i = 0; i < 2; ++i)
  {
    vec = get_base();
    decltype(vec) vec_1(3);
    nly::move(vec, vec_1.begin(), 0 == i);
    EXPECT_TRUE(nly::equal(vec_1, get_base()));
    EXPECT_TRUE(nly::all_of(vec, [](const std::string& item) { return item.empty(); }));
  }
}

TEST(Algorithm, Swap)
{
  {
    std::vector<int> vec_0{ 1, 2, 3 };
    std::vector<int> vec_1{ 4, 5 };

    nly::swap(vec_0, vec_1);
    EXPECT_TRUE(nly::equal(vec_0, std::vector<int>{ 4, 5 }));
    EXPECT_TRUE(nly::equal(vec_1, std::vector<int>{ 1, 2, 3 }));
  }

  {
    std::vector<int> vec_0{ 1, 2, 3 };
    std::vector<int> vec_1{ 4, 5 };

    nly::iter_swap(vec_0.begin(), vec_1.begin());
    EXPECT_TRUE(nly::equal(vec_0, std::vector<int>{ 4, 2, 3 }));
    EXPECT_TRUE(nly::equal(vec_1, std::vector<int>{ 1, 5 }));
  }

  for (int i = 0; i < 2; ++i)
  {
    std::vector<int> vec_0{ 4, 5 };
    std::vector<int> vec_1{ 1, 2, 3 };
    nly::swap_ranges(vec_0, vec_1.begin(), 0 == i);

    EXPECT_TRUE(nly::equal(vec_0, std::vector<int>{ 1, 2 }));
    EXPECT_TRUE(nly::equal(vec_1, std::vector<int>{ 4, 5, 3 }));
  }
}

TEST(Algorithm, Transform)
{
  for (int i = 0; i < 2; ++i)
  {

    std::vector<int> vec_0{ 1, 2, 3 };
    std::vector<int> vec_1(3);

    nly::transform(vec_0, vec_1.begin(), [](int a) { return ++a; }, 0 == i);
    EXPECT_TRUE(nly::equal(vec_1, std::vector<int>{ 2, 3, 4 }));
  }

  for (int i = 0; i < 2; ++i)
  {

    std::vector<int> vec_0{ 1, 2, 3 };
    std::vector<int> vec_1{ 4, 5, 6 };
    std::vector<int> vec_2(3);

    nly::transform(vec_0, vec_1, vec_2.begin(), [](int a, int b) { return a + b; }, 0 == i);
    EXPECT_TRUE(nly::equal(vec_2, std::vector<int>{ 5, 7, 9 }));
  }
}

TEST(Algorithm, Replace)
{
  auto get_base = []()
  {
    return std::vector<int>{ 0, 1, 1, 2, 2, 3 };
    ;
  };

  for (int i = 0; i < 2; ++i)
  {
    auto vec = get_base();
    nly::replace(vec, 1, 100, i == 0);
    EXPECT_TRUE(nly::equal(vec, std::vector<int>{ 0, 100, 100, 2, 2, 3 }));
  }

  for (int i = 0; i < 2; ++i)
  {
    auto vec = get_base();
    nly::replace_if(vec, [](int value) { return value % 2; }, 100, i == 0);
    EXPECT_TRUE(nly::equal(vec, std::vector<int>{ 0, 100, 100, 2, 2, 100 }));
  }

  for (int i = 0; i < 2; ++i)
  {
    auto          vec = get_base();
    decltype(vec) out(vec.size());
    nly::replace_copy(vec, out.begin(), 1, 100, i == 0);
    EXPECT_TRUE(nly::equal(out, std::vector<int>{ 0, 100, 100, 2, 2, 3 }));
    EXPECT_TRUE(nly::equal(vec, get_base()));
  }

  for (int i = 0; i < 2; ++i)
  {
    auto          vec = get_base();
    decltype(vec) out(vec.size());
    nly::replace_copy_if(vec, out.begin(), [](int value) { return value % 2; }, 100, i == 0);
    EXPECT_TRUE(nly::equal(out, std::vector<int>{ 0, 100, 100, 2, 2, 100 }));
    EXPECT_TRUE(nly::equal(vec, get_base()));
  }
}

TEST(Algorithm, Fill)
{
  for (int i = 0; i < 2; ++i)
  {
    std::vector<int> vec(5);
    nly::fill(vec, 10, i == 0);
    EXPECT_TRUE(nly::all_of(vec, [](int value) { return value == 10; }));
  }

  int  root = 0;
  auto fun = [&root]() { return ++root; };

  for (int i = 0; i < 2; ++i)
  {
    root = 0;
    std::vector<int> vec(5);
    nly::generate(vec, fun, i == 0);
    EXPECT_TRUE(nly::equal(vec, std::vector<int>{ 1, 2, 3, 4, 5 }));
  }
}
