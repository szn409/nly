#include "gtest/gtest.h"
#include "nly/pool.hpp"
#include "nly/stl_helper/time.hpp"
#include <future>
#include <array>

template<typename t_pool>
void check_size(t_pool& pool, size_t available, size_t used, size_t all)
{
  EXPECT_TRUE(pool.get_available_size() == available);
  EXPECT_TRUE(pool.get_used_size() == used);
  EXPECT_TRUE(pool.get_total_size() == all);
  EXPECT_TRUE(available + used == all);
};

TEST(Pool, GetOne)
{
  nly::pool<std::vector<int>> pool;
  check_size(pool, 0, 0, 0);

  auto obj = pool.get_one(false);
  EXPECT_TRUE(obj == nullptr);
  check_size(pool, 0, 0, 0);

  obj = pool.get_one();
  EXPECT_TRUE(obj);
  check_size(pool, 0, 1, 1);

  obj->resize(1024);
  pool.release_one(obj);
  check_size(pool, 1, 0, 1);

  obj = pool.get_one();
  EXPECT_TRUE(obj);
  check_size(pool, 0, 1, 1);
  EXPECT_TRUE(obj->size() == 1024);
}

TEST(Pool, ReleaseOne)
{
  nly::pool<std::vector<int>> pool;

  auto obj = pool.get_one();
  pool.release_one(obj);
  check_size(pool, 1, 0, 1);

  std::vector<decltype(obj)> tmp;
  for (int i = 0; i < 10; ++i)
  {
    tmp.emplace_back(pool.get_one());
    check_size(pool, 0, i + 1, i + 1);
  }

  int index = 0;
  for (auto item : tmp)
  {
    pool.release_one(item);
    ++index;
    check_size(pool, index, 10 - index, 10);
  }
}

TEST(Pool, ReleaseAll)
{
  nly::pool<std::vector<int>> pool;

  auto obj = pool.get_one();
  pool.release_all();
  check_size(pool, 1, 0, 1);

  std::vector<decltype(obj)> tmp;
  for (int i = 0; i < 10; ++i)
  {
    tmp.emplace_back(pool.get_one());
  }

  for (int i = 0; i < 5; ++i)
  {
    pool.release_one(tmp.at(i));
  }
  check_size(pool, 5, 5, 10);

  pool.release_all();
  check_size(pool, 10, 0, 10);
}

TEST(Pool, Resize)
{
  nly::pool<std::vector<int>> pool;

  EXPECT_TRUE(pool.resize(10));
  check_size(pool, 10, 0, 10);

  auto obj = pool.get_one();

  EXPECT_TRUE(pool.resize(15));
  check_size(pool, 14, 1, 15);

  EXPECT_TRUE(!pool.resize(10));
  check_size(pool, 14, 1, 15);

  pool.release_one(obj);
  EXPECT_TRUE(pool.resize(10));
  check_size(pool, 10, 0, 10);
}

TEST(Pool, ThreadSave)
{
  nly::pool_s<std::vector<int>> pool;
  const int                     count = 100000;

  auto add = [&pool, count](std::promise<std::vector<std::vector<int>*>>& task)
  {
    std::vector<std::vector<int>*> output;

    for (int i = 0; i < count; ++i)
    {
      output.emplace_back(pool.get_one());
    }

    task.set_value(std::move(output));
  };

  auto release = [&pool](std::vector<std::vector<int>*>& input)
  {
    std::for_each(
      input.begin(),
      input.end(),
      [&pool](std::vector<int>* obj) { pool.release_one(obj); });
  };

  std::vector<std::thread>                                  thds;
  std::vector<std::promise<std::vector<std::vector<int>*>>> tasks;
  std::vector<std::future<std::vector<std::vector<int>*>>>  futures;
  std::vector<std::vector<std::vector<int>*>>               results;

  const int thrad_count = 3;

  {
    tasks.reserve(thrad_count);

    for (int i = 0; i < thrad_count; ++i)
    {
      tasks.emplace_back();
      futures.emplace_back(tasks.back().get_future());
      thds.emplace_back(add, std::ref(tasks.back()));
    }

    for (int i = 0; i < thrad_count; ++i)
    {
      thds.at(i).join();
      results.emplace_back(std::move(futures.at(i).get()));
    }
    check_size(pool, 0, count * thrad_count, count * thrad_count);
  }

  {
    thds.clear();
    for (int i = 0; i < thrad_count; ++i)
    {
      thds.emplace_back(release, std::ref(results.at(i)));
    }
    for (int i = 0; i < thrad_count; ++i)
    {
      thds.at(i).join();
    }
    check_size(pool, count * thrad_count, 0, count * thrad_count);
  }
}

TEST(Pool, Efficiency)
{
  const int size = 1024 * 1024 * 500;

  auto start_time = nly::time_count::now();
  auto buff = new char[size];
  auto normal_cost_time = nly::time_count::time_diff(start_time);

  nly::pool<std::array<char, size>> pool;
  pool.resize(1);
  pool.release_all();

  start_time = nly::time_count::now();
  pool.get_one();
  auto pool_cost_time = nly::time_count::time_diff(start_time);
  EXPECT_TRUE(pool_cost_time * 10000 < normal_cost_time);

  delete[] buff;
}
