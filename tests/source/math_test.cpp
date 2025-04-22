#include "gtest/gtest.h"
#include "nly/math.hpp"

TEST(Math, FloatEqual)
{
  EXPECT_TRUE(!nly::math::float_equal(1.0, 1.001));
  EXPECT_TRUE(nly::math::float_equal(1.0, 1.001, 0.0011));

  EXPECT_TRUE(nly::math::float_equal(1.0, 1.0));
  EXPECT_TRUE(nly::math::float_equal(1.0f, 1.0f));
  EXPECT_TRUE(nly::math::float_equal(1.0f, 1.0));

  EXPECT_TRUE(!nly::math::float_equal(1.0, 1.000000001));
  EXPECT_TRUE(nly::math::float_equal(1.0, 1.00000000001));
}
