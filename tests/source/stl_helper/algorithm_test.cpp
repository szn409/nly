#include "nly/stl_helper/algorithm.hpp"
#include "gtest/gtest.h"

TEST(Algorithm, ForEach)
{
  for (int i = 0; i < 2; ++i)
  {
    std::vector<int> vec{ 0, 1, 2, 3 };
    nly::for_each(vec, [](int& value) { ++value; }, i == 0);
    EXPECT_EQ(vec, (std::vector<int>{ 1, 2, 3, 4 }));
  }
}

TEST(Algorithm, AllOf)
{
  std::vector<int> vec{ 0, 1, 2, 3 };

  for (int i = 0; i < 2; ++i)
  {
    EXPECT_TRUE(!nly::all_of(vec, [](int value) { return value > 1; }, i == 0));
    EXPECT_TRUE(nly::all_of(vec, [](int value) { return value > -1; }, i == 0));
    EXPECT_TRUE(nly::all_of(std::vector<int>{}, [](int value) { return value > -1; }, i == 0));
  }
}

TEST(Algorithm, AnyOf)
{
  std::vector<int> vec{ 0, 1, 2, 3 };

  for (int i = 0; i < 2; ++i)
  {
    EXPECT_TRUE(!nly::any_of(vec, [](int value) { return value > 5; }, i == 0));
    EXPECT_TRUE(nly::any_of(vec, [](int value) { return value > 2; }, i == 0));
    EXPECT_TRUE(!nly::any_of(std::vector<int>{}, [](int value) { return value > -1; }, i == 0));
  }
}

TEST(Algorithm, NoneOf)
{
  std::vector<int> vec{ 0, 1, 2, 3 };

  for (int i = 0; i < 2; ++i)
  {
    EXPECT_TRUE(nly::none_of(vec, [](int value) { return value > 5; }, i == 0));
    EXPECT_TRUE(!nly::none_of(vec, [](int value) { return value > 2; }, i == 0));
    EXPECT_TRUE(nly::none_of(std::vector<int>{}, [](int value) { return value > -1; }, i == 0));
  }
}

TEST(Algorithm, Find)
{
  std::vector<int> vec{ 0, 1, 2, 3, 2 };

  for (int i = 0; i < 2; ++i)
  {
    EXPECT_TRUE(nly::find(vec, 2, i == 0) - vec.begin() == 2);
    EXPECT_TRUE(nly::find(vec, 5, i == 0) == vec.end());
  }
}

TEST(Algorithm, FindIf)
{
  std::vector<int> vec{ 0, 1, 2, 3, 2 };

  for (int i = 0; i < 2; ++i)
  {
    EXPECT_TRUE(nly::find_if(vec, [](int value) { return value >= 2; }, i == 0) - vec.begin() == 2);
    EXPECT_TRUE(nly::find_if(vec, [](int value) { return value >= 200; }, i == 0) == vec.end());
  }
}

TEST(Algorithm, FindIfNot)
{
  for (int i = 0; i < 2; ++i)
  {
    std::vector<int> vec{ 0, 1, 2, 3, 1 };
    EXPECT_TRUE(
      nly::find_if_not(vec, [](int value) { return value <= 1; }, i == 0) - vec.begin() == 2);
    EXPECT_TRUE(nly::find_if_not(vec, [](int value) { return value >= 0; }, i == 0) == vec.end());
  }
}

TEST(Algorithm, FindEnd)
{
  for (int i = 0; i < 2; ++i)
  {
    std::vector<int>       vec_0{ 0, 1, 2, 3, 4, 0, 1, 2 };
    const std::vector<int> vec_1{ 0, 1 };
    EXPECT_TRUE(nly::find_end(vec_0, vec_1, i == 1) - vec_0.begin() == 5);
    EXPECT_TRUE(nly::find_end(vec_1, vec_0, i == 1) == vec_1.end());
  }

  for (int i = 0; i < 2; ++i)
  {
    const std::vector<int> vec_0{ 0, 1, 2, 3, 4, 0, 1, 2 };
    std::vector<int>       vec_1{ 100, 200 };
    EXPECT_TRUE(nly::find_end(vec_0, vec_1, i == 0) == vec_0.end());
  }

  for (int i = 0; i < 2; ++i)
  {
    auto fun = [](int a, int b) { return a + 3 == b; };

    const std::vector<int> vec_0{ 0, 1, 2, 3, 4, 0, 1, 2 };
    const std::vector<int> vec_1{ 3, 4 };
    EXPECT_TRUE(nly::find_end(vec_0, vec_1, fun, i == 0) - vec_0.begin() == 5);
  }

  for (int i = 0; i < 2; ++i)
  {
    auto fun = [](int a, int b) { return a + 3 == b; };

    std::vector<int> vec_0{ 0, 1, 2, 3, 4, 0, 1, 2 };
    std::vector<int> vec_1{ 300, 400 };
    EXPECT_TRUE(nly::find_end(vec_0, vec_1, fun, i == 0) == vec_0.end());
  }
}

TEST(Algorithm, FindFirstOf)
{
  {
    std::vector<int>       vec_0{ 0, 1, 2, 3, 4 };
    const std::vector<int> vec_1{ 3, 5 };

    for (int i = 0; i < 2; ++i)
    {
      EXPECT_TRUE(*nly::find_first_of(vec_0, vec_1, i == 0) == 3);
      EXPECT_TRUE(
        *nly::find_first_of(vec_0, vec_1, [](int a, int b) { return a - b == 1; }, i == 0) == 4);
    }
  }

  {
    const std::vector<int> vec_0{ 0, 1, 2, 3, 4 };
    std::vector<int>       vec_1{ 100, 200 };
    for (int i = 0; i < 2; ++i)
    {
      EXPECT_TRUE(nly::find_first_of(vec_0, vec_1, i == 0) == vec_0.end());
    }
  }
}

TEST(Algorithm, AdjacentFind)
{
  for (int i = 0; i < 2; ++i)
  {
    std::vector<int> vec{ 0, 1, 1, 2, 3, 3, 4 };
    EXPECT_TRUE(nly::adjacent_find(vec, i == 0) - vec.begin() == 1);
  }

  for (int i = 0; i < 2; ++i)
  {
    const std::vector<int> vec{ 0, 1, 3, 2, 3, 4 };
    EXPECT_TRUE(
      nly::adjacent_find(
        vec,
        [](int a, int b) { return a == 3 && b == 4; },
        i == 0) -
        vec.begin() ==
      4);
  }

  for (int i = 0; i < 2; ++i)
  {
    std::vector<int> vec{ 0, 1, 2, 3, 4 };
    EXPECT_TRUE(nly::adjacent_find(vec, i == 0) == vec.end());
  }
}

TEST(Algorithm, Count)
{
  std::vector<int> vec{ 0, 1, 2, 3, 4, 4 };
  for (int i = 0; i < 2; ++i)
  {
    EXPECT_TRUE(nly::count(vec, 4, i == 0) == 2);
    EXPECT_TRUE(nly::count_if(vec, [](int value) { return !(value % 2); }, i == 0) == 4);
  }
}

TEST(Algorithm, Mismatch)
{
  std::vector<int> vec_base{ 0, 1, 2, 3, 4 };
  std::vector<int> vec_1{ 0, 1, 2, 3 };
  std::vector<int> vec_2{ 0, 1, 2, 4, 5 };
  std::vector<int> vec_3(vec_base);

  for (int i = 0; i < 2; ++i)
  {
    auto res = nly::mismatch(vec_base, vec_1, i == 0);
    EXPECT_TRUE(res.first - vec_base.begin() == 4 && res.second == vec_1.end());
  }

  for (int i = 0; i < 2; ++i)
  {
    auto res = nly::mismatch(vec_base, vec_2, i == 0);
    EXPECT_TRUE(res.first - vec_base.begin() == 3 && res.second - vec_2.begin() == 3);
  }

  for (int i = 0; i < 2; ++i)
  {
    auto res = nly::mismatch(vec_base, vec_3, i == 0);
    EXPECT_TRUE(res.first == vec_base.end() && res.second == vec_3.end());
  }

  for (int i = 0; i < 2; ++i)
  {
    auto res = nly::mismatch(vec_1, vec_2, [](int a, int b) { return a <= b; }, i == 0);
    EXPECT_TRUE(res.first == vec_1.end() && res.second - vec_2.begin() == 4);
  }

  for (int i = 0; i < 2; ++i)
  {
    auto res = nly::mismatch(vec_base, vec_2, [](int a, int b) { return a <= b; }, i == 0);
    EXPECT_TRUE(res.first == vec_base.end() && res.second == vec_2.end());

    res = nly::mismatch(vec_base, vec_2, [](int a, int b) { return a >= b; }, i == 0);
    EXPECT_TRUE(res.first - vec_base.begin() == 3 && res.second - vec_2.begin() == 3);
  }
}

TEST(Algorithm, Equal)
{
  std::vector<int> vec_base{ 0, 1, 2, 3, 4 };
  std::vector<int> vec_1{ 0, 1, 2, 3 };
  std::vector<int> vec_2{ 0, 1, 2, 4, 5 };
  std::vector<int> vec_3(vec_base);

  for (int i = 0; i < 2; ++i)
  {
    EXPECT_TRUE(!nly::equal(vec_base, vec_1, i == 0));
    EXPECT_TRUE(!nly::equal(vec_base, vec_2, i == 0));
    EXPECT_TRUE(nly::equal(vec_base, vec_3, i == 0));
    EXPECT_TRUE(
      nly::equal(vec_base, vec_2, [](int a, int b) { return std::abs(a - b) <= 2; }, i == 0));
  }
}

TEST(Algorithm, Search)
{
  int value_1[] = { 0, 1, 2, 3, 4, 5, 1, 2, 3, 6 };
  int value_2[] = { 1, 2, 3 };
  int value_3[] = { 10, 20, 30 };

  for (int i = 0; i < 2; ++i)
  {
    EXPECT_TRUE(nly::search(value_1, value_2, i == 0) == value_1 + 1);
    EXPECT_TRUE(nly::search(value_1, value_3, i == 0) == std::end(value_1));
    EXPECT_TRUE(
      nly::search(
        value_1,
        value_3,
        [](int a, int b) { return a * 10 == b; },
        i == 0) == value_1 + 1);
  }

  const char str[] = "abc szn xyz szn";
  const char tmp[] = "szn";

  // 被搜索的是 's', 'z', 'n', '\0'
  for (int i = 0; i < 2; ++i)
  {
    EXPECT_TRUE(nly::search(str, tmp, i == 0) == str + 12);
  }
}

TEST(Algorithm, SearchN)
{
  int value[] = { 1, 2, 3, 1, 5, 7, 8, 8, 9 };

  for (int i = 0; i < 2; ++i)
  {
    EXPECT_TRUE(nly::search_n(value, 2, 8, i == 0) - value == 6);
    EXPECT_TRUE(
      nly::search_n(value, 3, 2, [](int a, int b) { return a >= b; }, i == 0) - value == 4);
  }
}

TEST(Algorithm, Copy)
{
  int  value[] = { 1, 2, 3, 4, 5 };
  auto fun = [](int value) { return value % 2; };

  for (int i = 0; i < 2; ++i)
  {
    std::vector<int> vec;
    nly::copy_if(value, vec, fun, i == 0);
    EXPECT_TRUE(nly::equal(std::vector<int>{ 1, 3, 5 }, vec));
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
    nly::swap_ranges(vec_0.begin(), vec_0.end(), vec_1.begin(), 0 == i);

    EXPECT_TRUE(nly::equal(vec_0, std::vector<int>{ 1, 2 }));
    EXPECT_TRUE(nly::equal(vec_1, std::vector<int>{ 4, 5, 3 }));
  }
}

TEST(Algorithm, Transform)
{
  for (int i = 0; i < 2; ++i)
  {
    std::vector<int> vec_0{ 1, 2, 3 };
    std::vector<int> vec_1;

    nly::transform(vec_0, vec_1, [](int a) { return ++a; }, 0 == i);
    EXPECT_TRUE(nly::equal(vec_1, std::vector<int>{ 2, 3, 4 }));
  }

  for (int i = 0; i < 2; ++i)
  {
    std::vector<int> vec_0{ 1, 2, 3 };
    std::vector<int> vec_1{ 4, 5, 6, 7 };
    std::vector<int> vec_2;

    nly::transform(vec_0, vec_1, vec_2, [](int a, int b) { return a + b; }, 0 == i);
    EXPECT_TRUE(nly::equal(vec_2, std::vector<int>{ 5, 7, 9 }));
  }

  for (int i = 0; i < 2; ++i)
  {
    std::vector<int> vec_0{ 1, 2, 3 };
    std::vector<int> vec_1{ 4, 5 };
    std::vector<int> vec_2;

    nly::transform(vec_0, vec_1, vec_2, [](int a, int b) { return a + b; }, 0 == i);
    EXPECT_TRUE(nly::equal(vec_2, std::vector<int>{ 5, 7 }));
  }
}

TEST(Algorithm, Replace)
{
  auto get_base = []() { return std::vector<int>{ 0, 1, 1, 2, 2, 3 }; };

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
    decltype(vec) out;
    nly::replace_copy(vec, out, 1, 100, i == 0);
    EXPECT_TRUE(nly::equal(out, std::vector<int>{ 0, 100, 100, 2, 2, 3 }));
    EXPECT_TRUE(nly::equal(vec, get_base()));
  }

  for (int i = 0; i < 2; ++i)
  {
    auto          vec = get_base();
    decltype(vec) out;
    nly::replace_copy_if(vec, out, [](int value) { return value % 2; }, 100, i == 0);
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

  std::atomic_int root = 0;
  auto            fun = [&root]() { return ++root; };

  for (int i = 0; i < 2; ++i)
  {
    root = 0;
    std::vector<int> vec(5);
    nly::generate(vec, fun, i == 0);
    EXPECT_TRUE(nly::equal(vec, std::vector<int>{ 1, 2, 3, 4, 5 }));
  }
}

TEST(Algorithm, Remove)
{
  auto get_base = []() { return std::vector<int>{ 0, 1, 1, 2, 2, 3 }; };

  for (int i = 0; i < 2; ++i)
  {
    auto vec = get_base();
    EXPECT_TRUE(2 == nly::remove(vec, 1, i == 0));
    EXPECT_TRUE(nly::equal(vec, std::vector<int>{ 0, 2, 2, 3 }));
  }

  for (int i = 0; i < 2; ++i)
  {
    auto vec = get_base();
    EXPECT_TRUE(3 == nly::remove_if(vec, [](int value) { return value % 2; }, i == 0));
    EXPECT_TRUE(nly::equal(vec, std::vector<int>{ 0, 2, 2 }));
  }

  for (int i = 0; i < 2; ++i)
  {
    auto          vec = get_base();
    decltype(vec) out;
    nly::remove_copy(vec, out, 1, i == 0);
    EXPECT_TRUE(nly::equal(vec, get_base()));
    EXPECT_TRUE(nly::equal(out, std::vector<int>{ 0, 2, 2, 3 }));
  }

  for (int i = 0; i < 2; ++i)
  {
    auto          vec = get_base();
    decltype(vec) out;
    nly::remove_copy_if(vec, out, [](int value) { return value % 2; }, i == 0);
    EXPECT_TRUE(nly::equal(vec, get_base()));
    EXPECT_TRUE(nly::equal(out, std::vector<int>{ 0, 2, 2 }));
  }
}

TEST(Algorithm, Unique)
{
  auto get_base = []() { return std::vector<int>{ 1, 2, 1, 1, 3, 3, 1, 1, 4, 5 }; };

  for (int i = 0; i < 2; ++i)
  {
    auto vec = get_base();
    nly::unique(vec, i == 0);
    EXPECT_TRUE(nly::equal(vec, std::vector<int>{ 1, 2, 1, 3, 1, 4, 5 }));
  }

  for (int i = 0; i < 2; ++i)
  {
    auto vec = get_base();
    nly::unique(vec, [](int a, int b) { return a == b; }, i == 0);
    EXPECT_TRUE(nly::equal(vec, std::vector<int>{ 1, 2, 1, 3, 1, 4, 5 }));
  }

  for (int i = 0; i < 2; ++i)
  {
    auto vec = std::vector<int>{ 1, 2, 4, 5, 10, 11, 12 };
    nly::unique(vec, [](int a, int b) { return a - b == -1; }, i == 0);
    EXPECT_TRUE(nly::equal(vec, std::vector<int>{ 1, 4, 10, 12 }));
  }

  for (int i = 0; i < 2; ++i)
  {
    auto vec = get_base();
    auto out = vec;
    nly::unique_copy(vec, out, i == 0);
    EXPECT_TRUE(nly::equal(vec, get_base()));
    EXPECT_TRUE(nly::equal(out, std::vector<int>{ 1, 2, 1, 3, 1, 4, 5 }));
  }

  for (int i = 0; i < 2; ++i)
  {
    auto vec = get_base();
    auto out = vec;
    nly::unique_copy(vec, out, [](int a, int b) { return a == b; }, i == 0);
    EXPECT_TRUE(nly::equal(vec, get_base()));
    EXPECT_TRUE(nly::equal(out, std::vector<int>{ 1, 2, 1, 3, 1, 4, 5 }));
  }

  for (int i = 0; i < 2; ++i)
  {
    auto vec = std::vector<int>{ 1, 2, 4, 5, 10, 11, 12 };
    auto out = vec;
    nly::unique_copy(vec, out, [](int a, int b) { return a - b == -1; }, i == 0);
    EXPECT_TRUE(nly::equal(vec, std::vector<int>{ 1, 2, 4, 5, 10, 11, 12 }));
    EXPECT_TRUE(nly::equal(out, std::vector<int>{ 1, 4, 10, 12 }));
  }
}

TEST(Algorithm, Reverse)
{
  auto get_base = []() { return std::vector<int>{ 1, 2, 3, 4 }; };

  for (int i = 0; i <= 2; ++i)
  {
    auto vec = get_base();
    nly::reverse(vec);
    EXPECT_TRUE(nly::equal(vec, std::vector<int>{ 4, 3, 2, 1 }));
  }

  for (int i = 0; i <= 2; ++i)
  {
    auto vec = get_base();
    auto out = vec;
    nly::reverse_copy(vec, out);
    EXPECT_TRUE(nly::equal(out, std::vector<int>{ 4, 3, 2, 1 }));
    EXPECT_TRUE(nly::equal(vec, get_base()));
  }
}

TEST(Algorithm, Sort)
{
  auto get_base = []()
  {
    std::vector<int> out(1000);
    nly::iota(out, 0);
    return out;
  };

  for (int i = 0; i < 2; ++i)
  {
    auto base = get_base();
    nly::shuffle(base);
    EXPECT_TRUE(base != get_base());

    nly::sort(base, i == 0);
    EXPECT_TRUE(base == get_base() && nly::is_sorted(base, i == 0));

    nly::shuffle(base);
    nly::sort(base, [](int a, int b) { return a < b; }, i == 0);
    EXPECT_TRUE(base == get_base() && nly::is_sorted(base, i == 0));

    nly::shuffle(base);
    nly::sort(base, [](int a, int b) { return a > b; }, i == 0);
    auto tmp = get_base();
    nly::reverse(tmp, i == 0);
    EXPECT_TRUE(tmp == base && nly::is_sorted(base, [](int a, int b) { return a > b; }, i == 0));
  }

  for (int i = 0; i < 2; ++i)
  {
    auto base = get_base();
    nly::shuffle(base);
    EXPECT_TRUE(base != get_base());

    nly::stable_sort(base, i == 0);
    EXPECT_TRUE(base == get_base() && nly::is_sorted(base, i == 0));

    nly::shuffle(base);
    nly::stable_sort(base, [](int a, int b) { return a < b; }, i == 0);
    EXPECT_TRUE(base == get_base() && nly::is_sorted(base, i == 0));

    nly::shuffle(base);
    nly::stable_sort(base, [](int a, int b) { return a > b; }, i == 0);
    auto tmp = get_base();
    nly::reverse(tmp, i == 0);
    EXPECT_TRUE(tmp == base && nly::is_sorted(base, [](int a, int b) { return a > b; }, i == 0));
  }
}

TEST(Algorithm, Merge)
{
  const auto base_0 = std::vector<int>{ 1, 2, 4 };
  const auto base_1 = std::vector<int>{ 3, 5, 7, 9 };

  for (int i = 0; i < 2; ++i)
  {
    std::vector<int> out;
    nly::merge(base_0, base_1, out, i == 0);
    EXPECT_TRUE(nly::is_sorted(out));
    EXPECT_TRUE(out == (std::vector<int>{ 1, 2, 3, 4, 5, 7, 9 }));
  }

  for (int i = 0; i < 2; ++i)
  {
    std::vector<int> out;
    nly::merge(base_0, base_1, out, [](int a, int b) { return a < b; }, i == 0);
    EXPECT_TRUE(nly::is_sorted(out));
    EXPECT_TRUE(out == (std::vector<int>{ 1, 2, 3, 4, 5, 7, 9 }));
  }

  for (int i = 0; i < 2; ++i)
  {
    auto tmp_0 = base_0;
    auto tmp_1 = base_1;

    auto fun = [](int a, int b) { return a > b; };
    nly::reverse(tmp_0);
    nly::reverse(tmp_1);

    std::vector<int> out;
    nly::merge(tmp_0, tmp_1, out, fun, i == 0);
    EXPECT_TRUE(nly::is_sorted(out, fun));
    EXPECT_TRUE(out == (std::vector<int>{ 9, 7, 5, 4, 3, 2, 1 }));
  }
}

TEST(Algorithm, Shuffle)
{
  auto get_base = []()
  {
    std::vector<int> out(1000);
    std::iota(out.begin(), out.end(), 0);
    return out;
  };

  int sum = 0;

  for (int i = 0; i < 5; ++i)
  {
    auto vec = get_base();
    nly::shuffle(vec);

    if (vec != get_base())
    {
      ++sum;
    }

    std::sort(vec.begin(), vec.end());
    EXPECT_TRUE(nly::equal(vec, get_base()));
  }

  EXPECT_TRUE(sum == 5);
}

TEST(Algorithm, Sample)
{
  auto get_base = []()
  {
    std::vector<int> out(1000);
    std::iota(out.begin(), out.end(), 0);
    return out;
  };

  auto                       base = get_base();
  std::set<std::vector<int>> out;

  for (int i = 0; i < 5; ++i)
  {
    std::vector<int> tmp;
    nly::sample(base, tmp, 5);
    out.insert(std::move(tmp));
  }
  EXPECT_TRUE(out.size() == 5);

  for (int i = 0; i < 10; ++i)
  {
    std::vector<int> tmp;
    nly::sample(base, tmp, base.size() + i);
    EXPECT_TRUE(tmp == base);
  }
}

TEST(Algorithm, RandomPick)
{
  {
    auto fun = [](auto& container)
    {
      container.resize(10);
      nly::iota(container, 0);

      std::map<int, int> count;

      const int pickTotalCount = 10000;
      for (int i = 0; i < pickTotalCount; ++i)
      {
        ++count[*nly::random_pick(container)];
      }
      EXPECT_TRUE(count.size() == container.size());

      for (auto [pickedValue, pickedCount] : count)
      {
        EXPECT_TRUE(
          std::abs(static_cast<double>(pickedCount) / pickTotalCount - 1.0 / count.size()) <= 0.05);
      }
    };

    std::vector<int> v0;
    std::list<int>   v1;
    fun(v0);
    fun(v1);
  }

  {
    const std::vector<int> v0;
    const std::list<int>   v1;

    EXPECT_TRUE(nly::random_pick(v0) == v0.cend());
    EXPECT_TRUE(nly::random_pick(v1) == v1.cend());
  }

  {
    auto fun = [](auto& container)
    {
      container.resize(10);
      nly::iota(container, 0);

      auto pred = [](int a) { return a % 2; };

      std::map<int, int> count;

      const int pickTotalCount = 10000;
      for (int i = 0; i < pickTotalCount; ++i)
      {
        ++count[*nly::random_pick(container, pred)];
      }
      EXPECT_TRUE(count.size() == container.size() / 2);

      for (auto [pickedValue, pickedCount] : count)
      {
        EXPECT_TRUE(
          std::abs(static_cast<double>(pickedCount) / pickTotalCount - 1.0 / count.size()) <= 0.05);
        EXPECT_TRUE(pickedValue % 2);
      }
    };

    std::vector<int> v0;
    std::list<int>   v1;
    fun(v0);
    fun(v1);
  }

  {
    std::vector<int> v0(10);
    std::list<int>   v1(10);

    EXPECT_TRUE(nly::random_pick(v0, [](int a) { return a > 1024; }) == v0.end());
    EXPECT_TRUE(nly::random_pick(v1, [](int a) { return a > 1024; }) == v1.end());
  }
}

TEST(Algorithm, Partition)
{
  auto fun = [](int value) { return value >= 4; };
  auto get_base = []() { return std::vector<int>{ 1, 2, 3, 4, 5, 6, 7 }; };

  for (int i = 0; i < 2; ++i)
  {
    auto out = get_base();

    nly::shuffle(out);
    auto res = nly::partition(out, fun, i == 0);
    EXPECT_TRUE(nly::is_partitioned(out, fun, i == 0));
    EXPECT_TRUE(nly::partition_point(out, fun) == res);

    nly::shuffle(out);
    res = nly::stable_partition(out, fun, i == 0);
    EXPECT_TRUE(nly::is_partitioned(out, fun, i == 0));
    EXPECT_TRUE(nly::partition_point(out, fun) == res);

    std::vector<int> out_true;
    std::vector<int> out_false;
    nly::shuffle(out);
    nly::partition_copy(out, out_true, out_false, fun);
    EXPECT_TRUE(nly::all_of(out_true, fun));
    EXPECT_TRUE(nly::none_of(out_false, fun));
  }

  auto out = get_base();
  EXPECT_TRUE(*nly::partition_point(out, [](int value) { return value <= 4; }) == 5);
  EXPECT_TRUE(nly::partition_point(out, [](int value) { return value <= 100; }) == out.end());
}

TEST(Algorithm, EqualRange)
{
  {
    std::vector<int> vec{ 3, 1, 2, 4, 4, 7, 6, 5 };
    EXPECT_TRUE(nly::lower_bound(vec, 4) - vec.begin() == 3);
    EXPECT_TRUE(nly::upper_bound(vec, 4) - vec.begin() == 5);

    auto it = nly::equal_range(vec, 4);
    EXPECT_TRUE(it.first - vec.begin() == 3);
    EXPECT_TRUE(it.second - vec.begin() == 5);

    EXPECT_TRUE(nly::binary_search(vec, 4));
  }

  {
    std::vector<int> vec{ 7, 6, 5, 4, 4, 3, 2, 1 };

    auto fun = [](int a, int b) { return a > b; };

    EXPECT_TRUE(nly::lower_bound(vec, 4, fun) - vec.begin() == 3);
    EXPECT_TRUE(nly::upper_bound(vec, 4, fun) - vec.begin() == 5);

    auto it = nly::equal_range(vec, 4, fun);
    EXPECT_TRUE(it.first - vec.begin() == 3);
    EXPECT_TRUE(it.second - vec.begin() == 5);
    EXPECT_TRUE(nly::binary_search(vec, 4, fun));
  }
}

TEST(Algorithm, Cmp)
{
  EXPECT_TRUE(nly::max(1, 2) == 2);
  EXPECT_TRUE(nly::max({ 1, 3, 2, 5, 4 }) == 5);

  auto fun = [](int a, int b) { return a > b; };

  for (int i = 0; i < 2; ++i)
  {
    auto vec = std::vector<int>{ 1, 3, 2, 5, 4 };
    EXPECT_TRUE(*nly::max_element(vec, i == 0) == 5);

    vec = std::vector<int>{ 1, 3, 2, 5, 4 };
    EXPECT_TRUE(*nly::max_element(vec, fun, i == 0) == 1);
  }

  EXPECT_TRUE(nly::min(1, 2) == 1);
  EXPECT_TRUE(nly::min({ 1, 3, 2, 5, 4 }) == 1);

  for (int i = 0; i < 2; ++i)
  {
    auto vec = std::vector<int>{ 1, 3, 2, 5, 4 };
    EXPECT_TRUE(*nly::min_element(vec, i == 0) == 1);

    vec = std::vector<int>{ 1, 3, 2, 5, 4 };
    EXPECT_TRUE(*nly::min_element(vec, fun, i == 0) == 5);
  }

  {
    auto value = { 1, 3, 2, 5, 4 };
    auto res = nly::minmax(value);
    EXPECT_TRUE(res.first == 1 && res.second == 5);
  }

  for (int i = 0; i < 2; ++i)
  {
    auto tmp = std::vector<int>{ 1, 3, 2, 5, 4 };
    auto res = nly::minmax_element(tmp, i == 0);
    EXPECT_TRUE(*res.first == 1 && *res.second == 5);

    res = nly::minmax_element(tmp, fun, i == 0);
    EXPECT_TRUE(*res.first == 5 && *res.second == 1);
  }

  {
    EXPECT_TRUE(nly::clamp(-1, 0, 2) == 0);
    EXPECT_TRUE(nly::clamp(1, 0, 2) == 1);
    EXPECT_TRUE(nly::clamp(3, 0, 2) == 2);

    int   a = 10;
    int   b = 20;
    int   c = 30;
    auto& res = nly::clamp(a, b, c);
    EXPECT_TRUE(res == 20 && &res == &b);
  }

  {
    for (int i = 0; i < 2; ++i)
    {
      std::vector<int> vec_0{ 1, 10, 5 };
      decltype(vec_0)  vec_1{ 2, 4, 6 };
      EXPECT_TRUE(nly::lexicographical_compare(vec_0, vec_1, 0 == i));
    }

    for (int i = 0; i < 2; ++i)
    {
      std::vector<int> vec_0{ 1, 10, 5 };
      decltype(vec_0)  vec_1{ 1, 4, 6, 8 };
      EXPECT_TRUE(!nly::lexicographical_compare(vec_0, vec_1, 0 == i));
      EXPECT_TRUE(nly::lexicographical_compare(vec_0, vec_1, fun, 0 == i));
    }

    for (int i = 0; i < 2; ++i)
    {
      EXPECT_TRUE(!nly::lexicographical_compare(std::vector<int>{}, std::vector<int>{}, 0 == i));
    }
  }
}

TEST(Algorithm, Iota)
{
  std::vector<int> vec(5);
  nly::iota(vec, 1);
  EXPECT_TRUE(nly::equal(vec, std::vector<int>{ 1, 2, 3, 4, 5 }));
}

TEST(Algorithm, Accumulate)
{
  std::vector<int> vec(5);
  nly::iota(vec, 0);
  EXPECT_TRUE(nly::accumulate(vec, 0) == 0 + 1 + 2 + 3 + 4);
}

TEST(Algorithm, Set)
{
  {
    std::vector<int> vec_0{ 1, 2, 3, 4, 5 };
    std::vector<int> vec_1{ 1, 3, 5 };
    std::vector<int> vec_2{ 1, 3, 5, 7 };

    for (int i = 0; i < 2; ++i)
    {
      EXPECT_TRUE(nly::includes(vec_0, vec_1, i == 0));
      EXPECT_TRUE(!nly::includes(vec_0, vec_2, i == 0));
    }

    nly::reverse(vec_0);
    nly::reverse(vec_1);
    for (int i = 0; i < 2; ++i)
    {
      EXPECT_TRUE(nly::includes(vec_0, vec_1, [](int a, int b) { return a > b; }, i == 0));
    }
  }

  {
    std::vector<int> vec_0{ 1, 3, 3, 5 };
    std::vector<int> vec_1{ 2, 3, 3, 3, 4, 5 };

    for (int i = 0; i < 2; ++i)
    {
      decltype(vec_0) out;
      nly::set_union(vec_0, vec_1, out, i == 0);
      EXPECT_TRUE(nly::equal(out, std::vector<int>{ 1, 2, 3, 3, 3, 4, 5 }));
    }

    nly::reverse(vec_0);
    nly::reverse(vec_1);
    for (int i = 0; i < 2; ++i)
    {
      decltype(vec_0) out;
      nly::set_union(vec_0, vec_1, out, [](int a, int b) { return a > b; }, i == 0);
      EXPECT_TRUE(nly::equal(out, std::vector<int>{ 5, 4, 3, 3, 3, 2, 1 }));
    }
  }

  {
    std::vector<int> vec_0{ 1, 3, 3, 5 };
    std::vector<int> vec_1{ 2, 3, 3, 3, 5 };

    for (int i = 0; i < 2; ++i)
    {
      decltype(vec_0) out;
      nly::set_intersection(vec_0, vec_1, out);
      EXPECT_TRUE(nly::equal(out, std::vector<int>{ 3, 3, 5 }));
    }

    nly::reverse(vec_0);
    nly::reverse(vec_1);
    for (int i = 0; i < 2; ++i)
    {
      decltype(vec_0) out;
      nly::set_intersection(vec_0, vec_1, out, [](int a, int b) { return a > b; }, i == 0);
      EXPECT_TRUE(nly::equal(out, std::vector<int>{ 5, 3, 3 }));
    }
  }

  {
    std::vector<int> vec_0{ 1, 3, 3, 5 };
    std::vector<int> vec_1{ 2, 3, 5 };

    for (int i = 0; i < 2; ++i)
    {
      decltype(vec_0) out(vec_0.size() + vec_1.size());
      nly::set_difference(vec_0, vec_1, out);
      EXPECT_TRUE(nly::equal(out, std::vector<int>{ 1, 3 }));
    }

    nly::reverse(vec_0);
    nly::reverse(vec_1);
    for (int i = 0; i < 2; ++i)
    {
      decltype(vec_0) out;
      nly::set_difference(vec_0, vec_1, out, [](int a, int b) { return a > b; }, i == 0);
      EXPECT_TRUE(nly::equal(out, std::vector<int>{ 3, 1 }));
    }
  }

  {
    std::vector<int> vec_0{ 1, 3, 3, 4, 5 };
    std::vector<int> vec_1{ 2, 3, 4 };

    for (int i = 0; i < 2; ++i)
    {
      decltype(vec_0) out(vec_0.size() + vec_1.size());
      nly::set_symmetric_difference(vec_0, vec_1, out);
      EXPECT_TRUE(nly::equal(out, std::vector<int>{ 1, 2, 3, 5 }));
    }

    nly::reverse(vec_0);
    nly::reverse(vec_1);
    for (int i = 0; i < 2; ++i)
    {
      decltype(vec_0) out;
      nly::set_symmetric_difference(vec_0, vec_1, out, [](int a, int b) { return a > b; }, i == 0);
      EXPECT_TRUE(nly::equal(out, std::vector<int>{ 5, 3, 2, 1 }));
    }
  }
}

TEST(Algorithm, Permutation)
{
  std::string str = "abc";
  EXPECT_TRUE(nly::next_permutation(str));
  EXPECT_TRUE(str == "acb");

  str = "cba";
  EXPECT_TRUE(!nly::next_permutation(str));
  EXPECT_TRUE(str == "abc");

  str = "acb";
  EXPECT_TRUE(nly::next_permutation(str, [](char a, char b) { return a > b; }));
  EXPECT_TRUE(str == "abc");

  str = "acb";
  EXPECT_TRUE(nly::prev_permutation(str));
  EXPECT_TRUE(str == "abc");
  EXPECT_TRUE(!nly::prev_permutation(str));
  EXPECT_TRUE(str == "cba");

  str = "acb";
  EXPECT_TRUE(nly::prev_permutation(str, [](char a, char b) { return a > b; }));
  EXPECT_TRUE(str == "bac");

  std::string a = "abc";
  std::string b = "bca";
  EXPECT_TRUE(nly::is_permutation(a, b));
  EXPECT_TRUE(nly::is_permutation(b, a));
}
