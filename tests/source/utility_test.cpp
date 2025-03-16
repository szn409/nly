#include "nly/utility.hpp"
#include "gtest/gtest.h"

TEST(Utility, ReleaseGuard)
{
  int value = 0;

  {
    nly::release_guard guard([&value]() { ++value; });
  }
  EXPECT_EQ(value, 1);

  {
    try
    {
      nly::release_guard guard([&value]() { ++value; });

      throw(1);
    }
    catch (...)
    {
    }
  }
  EXPECT_EQ(value, 2);
}
