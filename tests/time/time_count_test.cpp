#include "gtest/gtest.h"
#include "nly/time/time_count.hpp"
#include <thread>

TEST(TimeCount, All)
{
  auto time_start = nly::now();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  auto time_after_sleep = nly::now();

  auto time_diff_s = nly::time_diff(time_start, time_after_sleep);
  EXPECT_TRUE(time_diff_s >= 0.07 && time_diff_s <= 0.13);
  time_diff_s = nly::time_diff(time_after_sleep, time_start);
  EXPECT_TRUE(time_diff_s <= -0.07 && time_diff_s >= -0.13);
  time_diff_s = nly::time_diff(time_start);
  EXPECT_TRUE(time_diff_s >= 0.08 && time_diff_s <= 0.13);

  auto time_diff_ms = nly::time_diff_ms(time_start, time_after_sleep);
  EXPECT_TRUE(time_diff_ms >= 70 && time_diff_ms <= 130);
  time_diff_ms = nly::time_diff_ms(time_after_sleep, time_start);
  EXPECT_TRUE(time_diff_ms <= -70 && time_diff_ms >= -130);
  time_diff_ms = nly::time_diff_ms(time_start);
  EXPECT_TRUE(time_diff_ms >= 70 && time_diff_ms <= 130);

  auto time_diff_us = nly::time_diff_us(time_start, time_after_sleep);
  EXPECT_TRUE(time_diff_us >= 70000 && time_diff_us <= 130000);
  time_diff_us = nly::time_diff_us(time_after_sleep, time_start);
  EXPECT_TRUE(time_diff_us <= -70000 && time_diff_us >= -130000);
  time_diff_us = nly::time_diff_us(time_start);
  EXPECT_TRUE(time_diff_us >= 70000 && time_diff_us <= 130000);
}