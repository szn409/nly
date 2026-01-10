#include "gtest/gtest.h"
#include "nly/stl_helper/time.hpp"
#include "nly/thread_pool.hpp"

TEST(ThreadPool, SubmitTask)
{
  nly::thread_pool pool(2);

  auto task = [](int a, int b)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    return a + b;
  };

  auto             start_time = nly::time_count::now();
  std::future<int> fu_0 = pool.submit_task(std::bind(task, 1, 2));
  std::future<int> fu_1 = pool.submit_task(std::bind(task, 2, 3));
  EXPECT_TRUE(fu_0.get() == 3);
  EXPECT_TRUE(fu_1.get() == 5);
  auto used_time = nly::time_count::time_diff(start_time);
  EXPECT_TRUE(used_time >= 0.4 && used_time <= 0.7);
}

TEST(ThreadPool, DetachTaskAndWait)
{
  nly::thread_pool pool(2);

  auto task = [](int& a)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    return ++a;
  };

  for (int i = 0; i < 2; ++i)
  {
    int input[2] = { 1, 2 };

    auto start_time = nly::time_count::now();
    pool.detach_task(std::bind(task, std::ref(input[0])));
    pool.detach_task(std::bind(task, input[1]));
    pool.wait();
    auto used_time = nly::time_count::time_diff(start_time);
    EXPECT_TRUE(used_time >= 0.4 && used_time <= 0.7);
    EXPECT_EQ(input[0], 2);
    EXPECT_EQ(input[1], 2);
  }

  int              tmp = 100;
  auto             start_time = nly::time_count::now();
  std::future<int> fu = pool.submit_task(std::bind(task, std::ref(tmp)));
  fu.wait();
  auto used_time = nly::time_count::time_diff(start_time);
  EXPECT_TRUE(used_time >= 0.4 && used_time <= 0.7);
  EXPECT_EQ(tmp, 101);
}

TEST(ThreadPool, DeadLock)
{
  nly::thread_pool pool(2);

  auto task = [&pool]()
  {
    try
    {
      pool.wait();
      return true;
    }
    catch (const std::exception&)
    {
      return false;
    }
  };
  std::future<bool> fu = pool.submit_task(task);
  EXPECT_TRUE(!fu.get());
}

TEST(ThreadPool, DetachTaskAndNotWait)
{
  nly::thread_pool* pool = new nly::thread_pool(2);

  auto task = [](int& a)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    return ++a;
  };

  int input[2] = { 1, 2 };

  auto start_time = nly::time_count::now();
  pool->detach_task(std::bind(task, std::ref(input[0])));
  pool->detach_task(std::bind(task, std::ref(input[1])));
  delete pool;
  auto used_time = nly::time_count::time_diff(start_time);
  EXPECT_TRUE(used_time >= 0.4 && used_time <= 0.7);
  EXPECT_EQ(input[0], 2);
  EXPECT_EQ(input[1], 3);
}

TEST(ThreadPool, Purge)
{
  nly::thread_pool pool(1);

  auto task = []() { std::this_thread::sleep_for(std::chrono::milliseconds(500)); };

  auto start_time = nly::time_count::now();
  auto fu = pool.submit_task(task);
  for (int i = 0; i < 3; ++i)
  {
    pool.detach_task(task);
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  pool.purge();
  fu.wait();
  pool.wait();
  auto used_time = nly::time_count::time_diff(start_time);
  EXPECT_TRUE(used_time >= 0.4 && used_time <= 0.7);
}

TEST(ThreadPool, Get)
{
  nly::thread_pool pool(3);
  EXPECT_TRUE(pool.get_thread_count() == 3);

  auto task = []() { std::this_thread::sleep_for(std::chrono::milliseconds(500)); };
  for (int i = 0; i < 5; ++i)
  {
    pool.detach_task(task);
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  EXPECT_TRUE(pool.get_tasks_running() == 3);
  EXPECT_TRUE(pool.get_tasks_queued() == 2);
  EXPECT_TRUE(pool.get_tasks_total() == 5);
}

TEST(ThreadPool, ThreadSafeTest)
{
  nly::thread_pool pool(3);
  std::atomic_int  value;
  const int        count = 500000;

  auto task = [&pool, &value, count]()
  {
    for (int i = 0; i < count; ++i)
    {
      pool.detach_task([&value]() { ++value; });
    }
  };

  for (int i = 0; i < 5; ++i)
  {
    pool.detach_task(task);
  }
  pool.wait();
  EXPECT_TRUE(value == count * 5);
}

/*
TEST(ThreadPool, SubmitLoop)
{
  int             input[3] = { 1, 2, 3 };
  BS::thread_pool pool(3);

  auto task = [&input](int index)
  {
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
      if (index % 2)
      {
          input[index] += 100;
      }
      else
      {
          input[index] += 1;
      }
  };

  auto start_time = nly::time_count::now();
  auto fu = pool.submit_loop(0, 3, task);
  fu.get();
  auto used_time = nly::time_count::time_diff(start_time);
  EXPECT_EQ(input[0], 2);
  EXPECT_EQ(input[1], 102);
  EXPECT_EQ(input[2], 4);
  EXPECT_TRUE(used_time >= 0.4 && used_time <= 0.7);
}

TEST(ThreadPool, DetachLoop)
{
  int             input[3] = { 1, 2, 3 };
  BS::thread_pool pool(3);

  auto task = [&input](int index)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    if (index % 2)
    {
      input[index] += 100;
    }
    else
    {
      input[index] += 1;
    }
  };

  auto start_time = nly::time_count::now();
  pool.detach_loop(0, 3, task);
  pool.wait();
  auto used_time = nly::time_count::time_diff(start_time);
  EXPECT_EQ(input[0], 2);
  EXPECT_EQ(input[1], 102);
  EXPECT_EQ(input[2], 4);
  EXPECT_TRUE(used_time >= 0.4 && used_time <= 0.7);
}
*/
