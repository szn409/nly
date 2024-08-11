#include "gtest/gtest.h"
#include "nly/memory_pool.hpp"
#include "nly/time/time_count.hpp"

TEST(MemoryPool, All)
{
  const int size = 1024 * 1024 * 200;

  auto start_time = nly::now();
  auto buff = new char[size];
  auto normal_cost_time = nly::time_diff(start_time);

  nly::memory_pool pool(size, 2);
  EXPECT_TRUE(pool.node_size() == size);

  auto p = pool.malloc();
  EXPECT_TRUE(p);
  EXPECT_TRUE(!pool.release_unused());

  pool.free(p);
  EXPECT_TRUE(pool.release_unused());

  pool.malloc();
  start_time = nly::now();
  pool.malloc();
  auto pool_cost_time = nly::time_diff(start_time);
  EXPECT_TRUE(pool_cost_time * 10 < normal_cost_time);

  EXPECT_TRUE(pool.release_all());
  delete[] buff;
}