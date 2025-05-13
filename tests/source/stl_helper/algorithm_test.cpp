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

  {
    int                    sum = 0;
    const std::vector<int> vec{ 0, 1, 2, 3 };
    nly::for_each(vec, [&sum](const int& value) { sum += value; });
    EXPECT_EQ(sum, 6);
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
  std::vector<int> vec{ 0, 1, 2, 3 };

  for (int i = 0; i < 2; ++i)
  {
    EXPECT_TRUE(*nly::find(vec, 2, i == 0) == 2);
    EXPECT_TRUE(nly::find(vec, 5, i == 0) == vec.end());
  }
}

TEST(Algorithm, FindIf)
{
  std::vector<int> vec{ 0, 1, 2, 3 };

  for (int i = 0; i < 2; ++i)
  {
    EXPECT_TRUE(*nly::find_if(vec, [](int value) { return value >= 2; }, i == 0) == 2);
    EXPECT_TRUE(nly::find_if(vec, [](int value) { return value >= 200; }, i == 0) == vec.end());
  }
}

TEST(Algorithm, FindIfNot)
{
  for (int i = 0; i < 2; ++i)
  {
    std::vector<int> vec{ 0, 1, 2, 3 };
    EXPECT_TRUE(*nly::find_if_not(vec, [](int value) { return value <= 2; }, i == 0) == 3);
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
      EXPECT_TRUE(nly::find_first_of(vec_0, vec_1, i == 0) - vec_0.begin() == 3);
      EXPECT_TRUE(
        nly::find_first_of(
          vec_0,
          vec_1,
          [](int a, int b) { return a - b == 1; },
          i == 0) -
          vec_0.begin() ==
        4);
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
    auto res = nly::mismatch(vec_base, vec_2, [](int a, int b) { return a <= b; }, i == 0);
    EXPECT_TRUE(res.first == vec_base.end() && res.second == vec_2.end());
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
  }

  const char str[] = "abc szn xyz szn";
  const char tmp[] = "szn";

  // 被搜索的是 's', 'z', 'n', '\0'
  EXPECT_TRUE(nly::search(str, tmp) == str + 12);
}

TEST(Algorithm, SearchN)
{
  int value[] = { 1, 2, 3, 1, 5, 7, 8, 8, 9 };

  for (int i = 0; i < 2; ++i)
  {
    EXPECT_TRUE(nly::search_n(value, 2, 8, i == 0) - value == 6);
    EXPECT_TRUE(
      nly::search_n(value, 3, 2, [](int a, int b) { return a >= b; }, i == 0) - value == 4);

    return;
  }
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

  auto fun = [](int value) { return value % 2; };

  for (int i = 0; i < 2; ++i)
  {
    reset();
    nly::copy(value, vec.begin(), i == 0);
    EXPECT_TRUE(nly::equal(value, vec));

    reset();
    vec.erase(nly::copy_if(value, vec.begin(), fun, i == 0), vec.end());
    EXPECT_TRUE(nly::equal(std::vector<int>{ 1, 3, 5 }, vec));
  }

  reset();
  nly::copy_backward(value, vec.end());
  EXPECT_TRUE(nly::equal(value, vec));

  {
    std::vector<int> tmp{ 1, 2, 3, 4, 5 };
    std::copy_backward(tmp.begin(), tmp.begin() + 3, tmp.end());
    EXPECT_TRUE(nly::equal(tmp, std::vector<int>{ 1, 2, 1, 2, 3 }));

    std::vector<int> tmp1{ 1, 2, 3, 4, 5 };
    std::copy(tmp1.begin() + 3, tmp1.end(), tmp1.begin());
    EXPECT_TRUE(nly::equal(tmp1, std::vector<int>{ 4, 5, 3, 4, 5 }));
  }
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

  {
    vec = get_base();
    decltype(vec) vec_1(3);
    nly::move_backward(vec, vec_1.end());
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

    nly::transform(vec_0, vec_1.begin(), vec_2.begin(), [](int a, int b) { return a + b; }, 0 == i);
    EXPECT_TRUE(nly::equal(vec_2, std::vector<int>{ 5, 7, 9 }));
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

TEST(Algorithm, Remove)
{
  auto get_base = []() { return std::vector<int>{ 0, 1, 1, 2, 2, 3 }; };

  for (int i = 0; i < 2; ++i)
  {
    auto vec = get_base();
    vec.erase(nly::remove(vec, 1, i == 0), vec.end());
    EXPECT_TRUE(nly::equal(vec, std::vector<int>{ 0, 2, 2, 3 }));
  }

  for (int i = 0; i < 2; ++i)
  {
    auto vec = get_base();
    vec.erase(nly::remove_if(vec, [](int value) { return value % 2; }, i == 0), vec.end());
    EXPECT_TRUE(nly::equal(vec, std::vector<int>{ 0, 2, 2 }));
  }

  for (int i = 0; i < 2; ++i)
  {
    auto          vec = get_base();
    decltype(vec) out(vec.size());
    out.erase(nly::remove_copy(vec, out.begin(), 1, i == 0), out.end());
    EXPECT_TRUE(nly::equal(vec, get_base()));
    EXPECT_TRUE(nly::equal(out, std::vector<int>{ 0, 2, 2, 3 }));
  }

  for (int i = 0; i < 2; ++i)
  {
    auto          vec = get_base();
    decltype(vec) out(vec.size());
    out.erase(
      nly::remove_copy_if(
        vec,
        out.begin(),
        [](int value) { return value % 2; },
        i == 0),
      out.end());
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
    vec.erase(nly::unique(vec, i == 0), vec.end());
    EXPECT_TRUE(nly::equal(vec, std::vector<int>{ 1, 2, 1, 3, 1, 4, 5 }));
  }

  for (int i = 0; i < 2; ++i)
  {
    auto vec = get_base();
    vec.erase(nly::unique(vec, [](int a, int b) { return a == b; }, i == 0), vec.end());
    EXPECT_TRUE(nly::equal(vec, std::vector<int>{ 1, 2, 1, 3, 1, 4, 5 }));
  }

  for (int i = 0; i < 2; ++i)
  {
    auto vec = get_base();
    auto out = vec;
    out.erase(nly::unique_copy(vec, out.begin(), i == 0), out.end());
    EXPECT_TRUE(nly::equal(vec, get_base()));
    EXPECT_TRUE(nly::equal(out, std::vector<int>{ 1, 2, 1, 3, 1, 4, 5 }));
  }

  for (int i = 0; i < 2; ++i)
  {
    auto vec = get_base();
    auto out = vec;
    out.erase(
      nly::unique_copy(
        vec,
        out.begin(),
        [](int a, int b) { return a == b; },
        i == 0),
      out.end());
    EXPECT_TRUE(nly::equal(vec, get_base()));
    EXPECT_TRUE(nly::equal(out, std::vector<int>{ 1, 2, 1, 3, 1, 4, 5 }));
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
    nly::reverse_copy(vec, out.begin());
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

    nly::sort(base);
    EXPECT_TRUE(base == get_base() && nly::is_sorted(base));

    nly::shuffle(base);
    nly::sort(base, [](int a, int b) { return a < b; });
    EXPECT_TRUE(base == get_base() && nly::is_sorted(base));

    nly::shuffle(base);
    nly::sort(base, [](int a, int b) { return a > b; });
    auto tmp = get_base();
    nly::reverse(tmp);
    EXPECT_TRUE(tmp == base && nly::is_sorted(base, [](int a, int b) { return a > b; }));
  }

  for (int i = 0; i < 2; ++i)
  {
    auto base = get_base();
    nly::shuffle(base);
    EXPECT_TRUE(base != get_base());

    nly::stable_sort(base);
    EXPECT_TRUE(base == get_base() && nly::is_sorted(base));

    nly::shuffle(base);
    nly::stable_sort(base, [](int a, int b) { return a < b; });
    EXPECT_TRUE(base == get_base() && nly::is_sorted(base));

    nly::shuffle(base);
    nly::stable_sort(base, [](int a, int b) { return a > b; });
    auto tmp = get_base();
    nly::reverse(tmp);
    EXPECT_TRUE(tmp == base && nly::is_sorted(base, [](int a, int b) { return a > b; }));
  }
}

TEST(Algorithm, Merge)
{
  const auto base_0 = std::vector<int>{ 1, 2, 3 };
  const auto base_1 = std::vector<int>{ 3, 5, 7, 9 };

  for (int i = 0; i < 2; ++i)
  {
    std::vector<int> out(base_0.size() + base_1.size());
    nly::merge(base_0, base_1, out.begin(), i == 0);
    EXPECT_TRUE(nly::is_sorted(out));
  }

  for (int i = 0; i < 2; ++i)
  {
    std::vector<int> out(base_0.size() + base_1.size());
    nly::merge(base_0, base_1, out.begin(), [](int a, int b) { return a < b; }, i == 0);
    EXPECT_TRUE(nly::is_sorted(out));
  }

  for (int i = 0; i < 2; ++i)
  {
    auto tmp_0 = base_0;
    auto tmp_1 = base_1;

    auto fun = [](int a, int b) { return a > b; };
    nly::reverse(tmp_0);
    nly::reverse(tmp_1);

    std::vector<int> out(base_0.size() + base_1.size());
    nly::merge(tmp_0, tmp_1, out.begin(), fun, i == 0);
    EXPECT_TRUE(nly::is_sorted(out, fun));
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
    nly::sample(base, std::back_inserter(tmp), 5);
    out.insert(std::move(tmp));
  }
  EXPECT_TRUE(out.size() == 5);

  for (int i = 0; i < 10; ++i)
  {
    std::vector<int> tmp;
    nly::sample(base, std::back_inserter(tmp), base.size() + i);
    EXPECT_TRUE(tmp == base);
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
    nly::stable_partition(out, fun, i == 0);
    EXPECT_TRUE(nly::is_partitioned(out, fun, i == 0));

    auto out_true = get_base();
    auto out_false = get_base();
    nly::shuffle(out);
    auto it = nly::partition_copy(out, out_true.begin(), out_false.begin(), fun);
    out_true.erase(it.first, out_true.end());
    out_false.erase(it.second, out_false.end());
    EXPECT_TRUE(nly::all_of(out_true, fun));
    EXPECT_TRUE(nly::none_of(out_false, fun));
  }

  auto out = get_base();
  EXPECT_TRUE(*nly::partition_point(out, [](int value) { return value <= 4; }) == 5);
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

  for (int i = 0; i < 2; ++i)
  {
    EXPECT_TRUE(*nly::max_element(std::vector<int>{ 1, 3, 2, 5, 4 }, i == 0) == 5);
  }

  EXPECT_TRUE(nly::min(1, 2) == 1);
  EXPECT_TRUE(nly::min({ 1, 3, 2, 5, 4 }) == 1);

  for (int i = 0; i < 2; ++i)
  {
    EXPECT_TRUE(*nly::min_element(std::vector<int>{ 1, 3, 2, 5, 4 }, i == 0) == 1);
  }

  {
    auto res = nly::minmax({ 1, 3, 2, 5, 4 });
    EXPECT_TRUE(res.first == 1 && res.second == 5);
  }

  for (int i = 0; i < 2; ++i)
  {
    auto tmp = std::vector<int>{ 1, 3, 2, 5, 4 };
    auto res = nly::minmax_element(tmp, i == 0);
    EXPECT_TRUE(*res.first == 1 && *res.second == 5);
  }

  EXPECT_TRUE(nly::clamp(-1, 0, 2) == 0);
  EXPECT_TRUE(nly::clamp(1, 0, 2) == 1);
  EXPECT_TRUE(nly::clamp(3, 0, 2) == 2);

  for (int i = 0; i < 2; ++i)
  {
    std::vector<int> vec_0{ 1, 10, 5 };
    decltype(vec_0)  vec_1{ 2, 4, 6, 8 };
    EXPECT_TRUE(nly::lexicographical_compare(vec_0, vec_1, 0 == i));
  }

  for (int i = 0; i < 2; ++i)
  {
    std::vector<int> vec_0{ 1, 10, 5 };
    decltype(vec_0)  vec_1{ 1, 4, 6, 8 };
    EXPECT_TRUE(!nly::lexicographical_compare(vec_0, vec_1, 0 == i));
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
  EXPECT_TRUE(nly::accumulate(vec, 0) == 10);
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
    std::vector<int> vec_1{ 2, 3, 3, 3, 4 };

    for (int i = 0; i < 2; ++i)
    {
      decltype(vec_0) out(vec_0.size() + vec_1.size());
      out.erase(nly::set_union(vec_0, vec_1, out.begin()), out.end());
      EXPECT_TRUE(nly::equal(out, std::vector<int>{ 1, 2, 3, 3, 3, 4, 5 }));
    }
  }

  {
    std::vector<int> vec_0{ 1, 3, 3, 5 };
    std::vector<int> vec_1{ 2, 3, 3, 3, 5 };

    for (int i = 0; i < 2; ++i)
    {
      decltype(vec_0) out(vec_0.size() + vec_1.size());
      out.erase(nly::set_intersection(vec_0, vec_1, out.begin()), out.end());
      EXPECT_TRUE(nly::equal(out, std::vector<int>{ 3, 3, 5 }));
    }
  }

  {
    std::vector<int> vec_0{ 1, 3, 3, 5 };
    std::vector<int> vec_1{ 2, 3, 5 };

    for (int i = 0; i < 2; ++i)
    {
      decltype(vec_0) out(vec_0.size() + vec_1.size());
      out.erase(nly::set_difference(vec_0, vec_1, out.begin()), out.end());
      EXPECT_TRUE(nly::equal(out, std::vector<int>{ 1, 3 }));
    }
  }

  {
    std::vector<int> vec_0{ 1, 3, 3, 4, 5 };
    std::vector<int> vec_1{ 2, 3, 4 };

    for (int i = 0; i < 2; ++i)
    {
      decltype(vec_0) out(vec_0.size() + vec_1.size());
      out.erase(nly::set_symmetric_difference(vec_0, vec_1, out.begin()), out.end());
      EXPECT_TRUE(nly::equal(out, std::vector<int>{ 1, 2, 3, 5 }));
    }
  }
}

TEST(Algorithm, Permutation)
{
  std::string str = "abc";
  nly::next_permutation(str);
  EXPECT_TRUE(str == "acb");

  nly::next_permutation(str, [](char a, char b) { return a > b; });
  EXPECT_TRUE(str == "abc");

  str = "acb";
  nly::prev_permutation(str);
  EXPECT_TRUE(str == "abc");

  str = "acb";
  nly::prev_permutation(str, [](char a, char b) { return a > b; });
  EXPECT_TRUE(str == "bac");

  std::string a = "abc";
  std::string b = "bca";
  EXPECT_TRUE(nly::is_permutation(a, b));
}
