#include "gtest/gtest.h"
#include "BS_thread_pool.hpp"
#include "nly/time/time_count.hpp"

TEST(ThreadPool, SubmitTask)
{
  BS::thread_pool pool(2);

  auto task = [](int a, int b)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    return a + b;
  };

  auto             start_time = nly::now();
  std::future<int> fu_0 = pool.submit_task(std::bind(task, 1, 2));
  std::future<int> fu_1 = pool.submit_task(std::bind(task, 2, 3));
  EXPECT_TRUE(fu_0.get() == 3);
  EXPECT_TRUE(fu_1.get() == 5);
  auto used_time = nly::time_diff(start_time);
  EXPECT_TRUE(used_time >= 0.7 && used_time <= 1.3);
}

TEST(ThreadPool, SubmitLoop)
{
  int             input[3] = { 1, 2, 3 };
  BS::thread_pool pool(3);

  auto task = [&input](int index)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    if (index % 2)
    {
      input[index] += 100;
    }
    else
    {
      input[index] += 1;
    }
  };

  auto start_time = nly::now();
  auto fu = pool.submit_loop(0, 3, task);
  fu.get();
  auto used_time = nly::time_diff(start_time);
  EXPECT_EQ(input[0], 2);
  EXPECT_EQ(input[1], 102);
  EXPECT_EQ(input[2], 4);
  EXPECT_TRUE(used_time >= 0.7 && used_time <= 1.3);
}

TEST(ThreadPool, DetachTaskAndWait)
{
  BS::thread_pool pool(2);

  auto task = [](int& a)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    return ++a;
  };

  int input[2] = { 1, 2 };

  auto start_time = nly::now();
  pool.detach_task(std::bind(task, std::ref(input[0])));
  pool.detach_task(std::bind(task, input[1]));
  pool.wait();
  auto used_time = nly::time_diff(start_time);
  EXPECT_TRUE(used_time >= 0.7 && used_time <= 1.3);
  EXPECT_EQ(input[0], 2);
  EXPECT_EQ(input[1], 2);
}

TEST(ThreadPool, DetachTaskAndNotWait)
{
  BS::thread_pool* pool = new BS::thread_pool(2);

  auto task = [](int& a)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    return ++a;
  };

  int input[2] = { 1, 2 };

  auto start_time = nly::now();
  pool->detach_task(std::bind(task, std::ref(input[0])));
  pool->detach_task(std::bind(task, input[1]));
  delete pool;
  auto used_time = nly::time_diff(start_time);
  EXPECT_TRUE(used_time >= 0.7 && used_time <= 1.3);
  EXPECT_EQ(input[0], 2);
  EXPECT_EQ(input[1], 2);
}

TEST(ThreadPool, DetachLoop)
{
  int             input[3] = { 1, 2, 3 };
  BS::thread_pool pool(3);

  auto task = [&input](int index)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    if (index % 2)
    {
      input[index] += 100;
    }
    else
    {
      input[index] += 1;
    }
  };

  auto start_time = nly::now();
  pool.detach_loop(0, 3, task);
  pool.wait();
  auto used_time = nly::time_diff(start_time);
  EXPECT_EQ(input[0], 2);
  EXPECT_EQ(input[1], 102);
  EXPECT_EQ(input[2], 4);
  EXPECT_TRUE(used_time >= 0.7 && used_time <= 1.3);
}

TEST(ThreadPool, Purge)
{
  BS::thread_pool pool(1);

  auto task = []() { std::this_thread::sleep_for(std::chrono::milliseconds(1000)); };

  auto start_time = nly::now();
  for (int i = 0; i < 3; ++i)
  {
    pool.detach_task(task);
  }
  pool.wait();
  auto used_time = nly::time_diff(start_time);
  EXPECT_TRUE(used_time >= 2.7 && used_time <= 3.3);

  start_time = nly::now();
  auto fu = pool.submit_task(task);
  for (int i = 0; i < 3; ++i)
  {
    pool.detach_task(task);
  }
  fu.wait();
  pool.purge();
  pool.wait();
  used_time = nly::time_diff(start_time);
  EXPECT_TRUE(used_time >= 0.7 && used_time <= 2.3);
}