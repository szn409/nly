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

TEST(Math, ToRad)
{
  EXPECT_TRUE(nly::math::float_equal(nly::math::to_rad(0), 0));
  EXPECT_TRUE(nly::math::float_equal(nly::math::to_rad(30), 0.5235987755982988));
  EXPECT_TRUE(nly::math::float_equal(nly::math::to_rad(90), 1.5707963267948966));
  EXPECT_TRUE(nly::math::float_equal(nly::math::to_rad(150), 2.6179938779914944));
  EXPECT_TRUE(nly::math::float_equal(nly::math::to_rad(180), M_PI));
  EXPECT_TRUE(nly::math::float_equal(nly::math::to_rad(210), 3.6651914291880923));
  EXPECT_TRUE(nly::math::float_equal(nly::math::to_rad(270), 4.71238898038469));
  EXPECT_TRUE(nly::math::float_equal(nly::math::to_rad(310), 5.410520681182422));
  EXPECT_TRUE(nly::math::float_equal(nly::math::to_rad(360), M_PI * 2));
  EXPECT_TRUE(nly::math::float_equal(nly::math::to_rad(390), 6.806784082777885));
}
