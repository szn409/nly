#include "gtest/gtest.h"
#include "nly/cppitertools.hpp"
#include "nly/math.hpp"
#include "fmt/format.h"
#include <utility>
#include <vector>
#include <string>
#include <set>

class hello
{
public:
  hello(int& ref)
    : m_value{ &ref }
  {
    ++*m_value;
  }

  hello(hello& h)
  {
    m_value = h.m_value;
    ++*m_value;
  }

  hello(hello&& h)
  {
    m_value = h.m_value;
    ++*m_value;
  }

public:
  operator bool() const
  {
    return true;
  }

public:
  int get()
  {
    return *m_value;
  }

private:
  int* m_value;
};

TEST(Iter, Zip)
{
  {
    std::vector<int>         v0{ 1, 2, 3, 4 };
    std::set<std::string>    v1{ "hello", "world" };
    std::vector<std::string> result;

    for (auto&& [tmp0, tmp1] : nly::zip(v0, v1))
    {
      result.emplace_back(fmt::format("{}, {}", tmp0, tmp1));
    }

    EXPECT_TRUE(result == (std::vector<std::string>{ "1, hello", "2, world" }));
  }

  {
    int sum = 0;
    for (auto&& [v0, v1] : nly::zip(std::vector<int>{ 2, 3, 4 }, std::vector<int>{ 4, 5 }))
    {
      sum += v0 * v1;
    }
    EXPECT_TRUE(sum == 2 * 4 + 3 * 5);
  }

  {
    int count = 0;

    std::vector<hello> v0;
    std::vector<int>   v1{ 1, 2, 3 };

    v0.emplace_back(count);
    v0.emplace_back(count);
    count = 0;

    int sum = 0;
    for (auto&& [tmp0, tmp1] : nly::zip(v0, v1))
    {
      sum += tmp0.get();
      sum += tmp1;
    }

    EXPECT_TRUE(0 == count && 3 == sum);
  }
}

TEST(Iter, Range)
{
  std::vector<int> vec;
  for (auto v : nly::range(5))
  {
    vec.emplace_back(v);
  }
  EXPECT_TRUE(vec == (std::vector<int>{ 0, 1, 2, 3, 4 }));

  vec.clear();
  for (auto v : nly::range(1, 5))
  {
    vec.emplace_back(v);
  }
  EXPECT_TRUE(vec == (std::vector<int>{ 1, 2, 3, 4 }));

  vec.clear();
  for (auto v : nly::range(1, 5, 2))
  {
    vec.emplace_back(v);
  }
  EXPECT_TRUE(vec == (std::vector<int>{ 1, 3 }));

  vec.clear();
  for (auto v : nly::range(1, -5, -1))
  {
    vec.emplace_back(v);
  }
  EXPECT_TRUE(vec == (std::vector<int>{ 1, 0, -1, -2, -3, -4 }));

  vec.clear();
  for (auto v : nly::range(1, -5, -2))
  {
    vec.emplace_back(v);
  }
  EXPECT_TRUE(vec == (std::vector<int>{ 1, -1, -3 }));

  vec.clear();
  for (auto v : nly::range(0, 5, 0))
  {
    vec.emplace_back(v);
  }
  EXPECT_TRUE(vec.empty());

  double sum = 0;
  for (auto v : nly::range(0.0, 4.9))
  {
    sum += v;
  }
  EXPECT_TRUE(nly::math::float_equal(sum, 10.0));
}

TEST(Iter, Enumerate)
{
  {
    std::vector<std::string>                    vec{ "hello", "world", "szn" };
    std::vector<std::pair<size_t, std::string>> result;

    for (auto&& [index, value] : nly::enumerate(vec))
    {
      result.emplace_back(index, value);
    }

    EXPECT_TRUE(
      result == (std::vector<std::pair<size_t, std::string>>{
                  { 0, "hello" },
                  { 1, "world" },
                  { 2, "szn" },
                }));
  }

  {
    int count = 0;

    std::vector<hello> vec;
    vec.emplace_back(count);
    vec.emplace_back(count);
    vec.emplace_back(count);

    std::vector<std::pair<size_t, int>> result;

    count = 0;
    for (auto&& [index, value] : nly::enumerate(vec))
    {
      result.emplace_back(index, value.get());
    }

    EXPECT_TRUE(
      result == (std::vector<std::pair<size_t, int>>{
                  { 0, 0 },
                  { 1, 0 },
                  { 2, 0 },
                }));
  }
}

TEST(Iter, Filter)
{
  {
    std::vector<std::string> vec{ "ab", "bcd", "1", "", "234", "5678" };
    decltype(vec)            result;

    for (auto&& v : nly::filter([](const std::string& value) { return value.size() > 2; }, vec))
    {
      result.emplace_back(v);
    }

    EXPECT_TRUE(result == (std::vector<std::string>{ "bcd", "234", "5678" }));
  }

  {
    std::vector<int> vec{ 0, 1, 0, 3, 0, 5 };
    std::vector<int> result;

    for (auto&& v : nly::filter(vec))
    {
      result.emplace_back(v);
    }
    EXPECT_TRUE(result == (std::vector<int>{ 1, 3, 5 }));
  }

  {
    int count = 0;

    std::vector<hello> vec;
    vec.emplace_back(count);
    vec.emplace_back(count);
    vec.emplace_back(count);

    count = 0;

    std::vector<int> result;

    for (auto&& v : nly::filter(vec))
    {
      result.emplace_back(v.get());
    }

    EXPECT_TRUE(result == (std::vector<int>{ 0, 0, 0 }));
  }
}

TEST(Iter, FilterFalse)
{
  {
    std::vector<std::string> vec{ "ab", "bcd", "1", "", "234", "5678" };
    decltype(vec)            result;

    for (auto&& v :
         nly::filterfalse([](const std::string& value) { return value.size() > 2; }, vec))
    {
      result.emplace_back(v);
    }

    EXPECT_TRUE(result == (std::vector<std::string>{ "ab", "1", "" }));
  }

  {
    std::vector<int> vec{ 0, 1, 0, 3, 0, 5 };
    std::vector<int> result;

    for (auto&& v : nly::filterfalse(vec))
    {
      result.emplace_back(v);
    }
    EXPECT_TRUE(result == (std::vector<int>{ 0, 0, 0 }));
  }

  {
    int count = 0;

    std::vector<hello> vec;
    vec.emplace_back(count);
    vec.emplace_back(count);
    vec.emplace_back(count);

    count = 0;

    std::vector<int> result;

    for (auto&& v : nly::filterfalse(vec))
    {
      result.emplace_back(v.get());
    }

    EXPECT_TRUE(result == (std::vector<int>{}));
  }
}

TEST(Iter, TakeWhile)
{
  {
    std::vector<std::string> vec{ "ab", "bcd", "1", "", "234", "5678" };
    decltype(vec)            result;

    for (auto&& v : nly::takewhile([](const std::string& value) { return value.size() >= 2; }, vec))
    {
      result.emplace_back(v);
    }

    EXPECT_TRUE(result == (std::vector<std::string>{ "ab", "bcd" }));
  }

  {
    std::vector<int> vec{ 1, 2, 0, 3, 0, 5 };
    std::vector<int> result;

    for (auto&& v : nly::takewhile(vec))
    {
      result.emplace_back(v);
    }
    EXPECT_TRUE(result == (std::vector<int>{ 1, 2 }));
  }

  {
    int count = 0;

    std::vector<hello> vec;
    vec.emplace_back(count);
    vec.emplace_back(count);
    vec.emplace_back(count);

    count = 0;

    std::vector<int> result;

    for (auto&& v : nly::takewhile(vec))
    {
      result.emplace_back(v.get());
    }

    EXPECT_TRUE(result == (std::vector<int>{ 0, 0, 0 }));
  }
}

TEST(Iter, DropWhile)
{
  {
    std::vector<std::string> vec{ "ab", "1", "", "234", "5678", "", "2" };
    decltype(vec)            result;

    for (auto&& v : nly::dropwhile([](const std::string& value) { return value.size() <= 2; }, vec))
    {
      result.emplace_back(v);
    }

    EXPECT_TRUE(result == (std::vector<std::string>{ "234", "5678", "", "2" }));
  }

  {
    std::vector<int> vec{ 1, 2, 0, 3, 0, 5 };
    std::vector<int> result;

    for (auto&& v : nly::dropwhile(vec))
    {
      result.emplace_back(v);
    }
    EXPECT_TRUE(result == (std::vector<int>{ 0, 3, 0, 5 }));
  }

  {
    int count = 0;

    std::vector<hello> vec;
    vec.emplace_back(count);
    vec.emplace_back(count);
    vec.emplace_back(count);

    count = 0;

    std::vector<int> result;

    for (auto&& v : nly::dropwhile(vec))
    {
      result.emplace_back(v.get());
    }

    EXPECT_TRUE(result == (std::vector<int>{}));
  }
}

TEST(Iter, Cycle)
{
  {
    std::vector<int> vec{ 1, 2, 3 };
    std::vector<int> result;

    for (auto& v : nly::cycle(vec))
    {
      result.emplace_back(v);
      if (result.size() >= 8)
      {
        break;
      }
    }

    EXPECT_TRUE(result == (std::vector<int>{ 1, 2, 3, 1, 2, 3, 1, 2 }));
  }

  {
    int count = 0;

    std::vector<hello> vec;
    vec.emplace_back(count);
    vec.emplace_back(count);
    vec.emplace_back(count);

    count = 0;

    std::vector<int> result;

    for (auto&& v : nly::cycle(vec))
    {
      result.emplace_back(v.get());
      if (result.size() >= 4)
      {
        break;
      }
    }

    EXPECT_TRUE(result == (std::vector<int>{ 0, 0, 0, 0 }));
  }
}