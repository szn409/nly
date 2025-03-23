#include "nly/geometry.hpp"
#include "gtest/gtest.h"
#include <numeric>
#include <vector>

void checkMat(const cv::Mat input, int width, int height, const std::vector<int>& target)
{
  EXPECT_EQ(input.cols, width);
  EXPECT_EQ(input.rows, height);
  EXPECT_EQ(width * height, target.size());

  int* data = reinterpret_cast<int*>(input.data);

  int index = 0;
  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
    {
      EXPECT_EQ(input.at<int>(y, x), target[index++]);
    }
  }
}

TEST(Geometry, GetSpecificRegion)
{
  int value[12] = {};
  std::iota(value, value + 12, 0);

  cv::Mat data(4, 3, CV_32SC1, value);
  checkMat(data, 3, 4, std::vector<int>{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 });

  //  cut
  {
    // x 方向
    checkMat(nly::get_specific_region(data, cv::Rect(-4, 0, 4, 1), true), 0, 0, {});
    checkMat(nly::get_specific_region(data, cv::Rect(-3, 0, 4, 1), true), 1, 1, { 0 });
    checkMat(nly::get_specific_region(data, cv::Rect(-2, 0, 4, 1), true), 2, 1, { 0, 1 });
    checkMat(nly::get_specific_region(data, cv::Rect(-1, 0, 4, 1), true), 3, 1, { 0, 1, 2 });
    checkMat(nly::get_specific_region(data, cv::Rect(0, 0, 4, 1), true), 3, 1, { 0, 1, 2 });
    checkMat(nly::get_specific_region(data, cv::Rect(1, 0, 4, 1), true), 2, 1, { 1, 2 });
    checkMat(nly::get_specific_region(data, cv::Rect(2, 0, 4, 1), true), 1, 1, { 2 });
    checkMat(nly::get_specific_region(data, cv::Rect(3, 0, 4, 1), true), 0, 0, {});

    // y 方向
    checkMat(nly::get_specific_region(data, cv::Rect(0, -5, 1, 5), true), 0, 0, {});
    checkMat(nly::get_specific_region(data, cv::Rect(0, -4, 1, 5), true), 1, 1, { 0 });
    checkMat(nly::get_specific_region(data, cv::Rect(0, -3, 1, 5), true), 1, 2, { 0, 3 });
    checkMat(nly::get_specific_region(data, cv::Rect(0, -2, 1, 5), true), 1, 3, { 0, 3, 6 });
    checkMat(nly::get_specific_region(data, cv::Rect(0, -1, 1, 5), true), 1, 4, { 0, 3, 6, 9 });
    checkMat(nly::get_specific_region(data, cv::Rect(0, 0, 1, 5), true), 1, 4, { 0, 3, 6, 9 });
    checkMat(nly::get_specific_region(data, cv::Rect(0, 1, 1, 5), true), 1, 3, { 3, 6, 9 });
    checkMat(nly::get_specific_region(data, cv::Rect(0, 2, 1, 5), true), 1, 2, { 6, 9 });
    checkMat(nly::get_specific_region(data, cv::Rect(0, 3, 1, 5), true), 1, 1, { 9 });
    checkMat(nly::get_specific_region(data, cv::Rect(0, 4, 1, 5), true), 0, 0, {});

    // x y 综合
    checkMat(nly::get_specific_region(data, cv::Rect(-2, -2, 2, 2), true), 0, 0, {});
    checkMat(nly::get_specific_region(data, cv::Rect(-1, -1, 2, 2), true), 1, 1, { 0 });
    checkMat(nly::get_specific_region(data, cv::Rect(0, 0, 2, 2), true), 2, 2, { 0, 1, 3, 4 });
    checkMat(nly::get_specific_region(data, cv::Rect(1, 1, 2, 2), true), 2, 2, { 4, 5, 7, 8 });
    checkMat(nly::get_specific_region(data, cv::Rect(2, 2, 2, 2), true), 1, 2, { 8, 11 });
    checkMat(nly::get_specific_region(data, cv::Rect(3, 3, 2, 2), true), 0, 0, {});
  }

  // not cut
  {
    // x 方向
    for (int x = -10; x < 10; ++x)
    {
      checkMat(nly::get_specific_region(data, cv::Rect(x, 0, 4, 1), false), 3, 1, { 0, 1, 2 });
    }
    checkMat(nly::get_specific_region(data, cv::Rect(-2, 0, 2, 1), false), 2, 1, { 0, 1 });
    checkMat(nly::get_specific_region(data, cv::Rect(-1, 0, 2, 1), false), 2, 1, { 0, 1 });
    checkMat(nly::get_specific_region(data, cv::Rect(0, 0, 2, 1), false), 2, 1, { 0, 1 });
    checkMat(nly::get_specific_region(data, cv::Rect(1, 0, 2, 1), false), 2, 1, { 1, 2 });
    checkMat(nly::get_specific_region(data, cv::Rect(2, 0, 2, 1), false), 2, 1, { 1, 2 });
    checkMat(nly::get_specific_region(data, cv::Rect(3, 0, 2, 1), false), 2, 1, { 1, 2 });

    // y 方向
    for (int y = -10; y < 10; ++y)
    {
      checkMat(nly::get_specific_region(data, cv::Rect(0, y, 1, 4), false), 1, 4, { 0, 3, 6, 9 });
    }
    checkMat(nly::get_specific_region(data, cv::Rect(0, -4, 1, 3), false), 1, 3, { 0, 3, 6 });
    checkMat(nly::get_specific_region(data, cv::Rect(0, -3, 1, 3), false), 1, 3, { 0, 3, 6 });
    checkMat(nly::get_specific_region(data, cv::Rect(0, -2, 1, 3), false), 1, 3, { 0, 3, 6 });
    checkMat(nly::get_specific_region(data, cv::Rect(0, -1, 1, 3), false), 1, 3, { 0, 3, 6 });
    checkMat(nly::get_specific_region(data, cv::Rect(0, 0, 1, 3), false), 1, 3, { 0, 3, 6 });
    checkMat(nly::get_specific_region(data, cv::Rect(0, 1, 1, 3), false), 1, 3, { 3, 6, 9 });
    checkMat(nly::get_specific_region(data, cv::Rect(0, 2, 1, 3), false), 1, 3, { 3, 6, 9 });
    checkMat(nly::get_specific_region(data, cv::Rect(0, 3, 1, 3), false), 1, 3, { 3, 6, 9 });
    checkMat(nly::get_specific_region(data, cv::Rect(0, 4, 1, 3), false), 1, 3, { 3, 6, 9 });

    // x y 综合
    for (int i = -10; i <= 0; ++i)
    {
      checkMat(nly::get_specific_region(data, cv::Rect(i, i, 2, 2), false), 2, 2, { 0, 1, 3, 4 });
    }
    checkMat(nly::get_specific_region(data, cv::Rect(1, 1, 2, 2), false), 2, 2, { 4, 5, 7, 8 });
    for (int i = 2; i < 10; ++i)
    {
      checkMat(nly::get_specific_region(data, cv::Rect(2, 2, 2, 2), false), 2, 2, { 7, 8, 10, 11 });
    }
  }
}
