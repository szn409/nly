#include "nly/opencv_helper.hpp"
#include "gtest/gtest.h"
#include <numeric>
#include <vector>

template<typename T = int>
void checkMat(const cv::Mat input, int width, int height, const std::vector<T>& target)
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
      EXPECT_EQ(input.at<T>(y, x), target[index++]);
    }
  }
}

TEST(OpencvHelper, CreateMat)
{
  auto mat = nly::create_mat(4, 3, CV_8UC1);
  EXPECT_TRUE(mat.rows == 4 && mat.cols == 3 && mat.type() == CV_8UC1);
  checkMat(mat, 3, 4, std::vector<unsigned char>(12));

  mat = nly::create_mat(4, 3, CV_8UC(128));
  EXPECT_TRUE(mat.channels() == 128);

  mat = nly::create_mat(2, 2, CV_8UC3, cv::Scalar(10, 20, 30));
  int index = 0;
  for (int y = 0; y < 2; ++y)
  {
    for (int x = 0; x < 2; ++x)
    {
      EXPECT_EQ(mat.at<cv::Vec3b>(y, x)[0], 10);
      EXPECT_EQ(mat.at<cv::Vec3b>(y, x)[1], 20);
      EXPECT_EQ(mat.at<cv::Vec3b>(y, x)[2], 30);
    }
  }

  std::vector<unsigned char> value{ 1, 2, 3, 0, 4, 5, 6, 0 };
  mat = nly::create_mat(2, 3, CV_8UC1, value.data());
  checkMat(mat, 3, 2, std::vector<unsigned char>{ 1, 2, 3, 0, 4, 5 });

  mat = nly::create_mat(2, 3, CV_8UC1, value.data(), 4);
  checkMat(mat, 3, 2, std::vector<unsigned char>{ 1, 2, 3, 4, 5, 6 });

  value[0] = 255;
  checkMat(mat, 3, 2, std::vector<unsigned char>{ 255, 2, 3, 4, 5, 6 });
}

TEST(OpencvHelper, IsMatEqual)
{
  cv::Mat mat_0;
  cv::Mat mat_1;
  EXPECT_TRUE(nly::is_mat_equal(mat_0, mat_0));
  EXPECT_TRUE(nly::is_mat_equal(mat_0, mat_1));

  std::vector<unsigned char> value{ 1, 2, 3, 0, 4, 5, 6, 0 };
  mat_0 = nly::create_mat(2, 3, CV_8UC1, value.data());
  mat_1 = nly::create_mat(3, 2, CV_8UC1, value.data());
  EXPECT_TRUE(!nly::is_mat_equal(mat_0, mat_1));

  mat_0 = nly::create_mat(2, 3, CV_8UC1, value.data(), 4);
  mat_1 = nly::create_mat(2, 3, CV_8UC1, value.data(), 4);
  EXPECT_TRUE(nly::is_mat_equal(mat_0, mat_1));

  mat_0 = nly::create_mat(2, 3, CV_8UC1, value.data(), 4);
  mat_1 = nly::create_mat(2, 3, CV_8UC1, value.data());
  EXPECT_TRUE(!nly::is_mat_equal(mat_0, mat_1));

  mat_0 = nly::create_mat(2, 2, CV_8UC1);
  mat_1 = nly::create_mat(2, 2, CV_8UC3);
  EXPECT_TRUE(!nly::is_mat_equal(mat_0, mat_1));

  mat_0 = nly::create_mat(2, 2, CV_8UC1);
  mat_1 = nly::create_mat(2, 2, CV_8SC1);
  EXPECT_TRUE(!nly::is_mat_equal(mat_0, mat_1));

  mat_0 = nly::create_mat(2, 2, CV_8UC3);
  mat_1 = nly::create_mat(2, 2, CV_8UC3);
  EXPECT_TRUE(nly::is_mat_equal(mat_0, mat_1));

  mat_0 = nly::create_mat(2, 2, CV_8UC3);
  mat_1 = nly::create_mat(2, 2, CV_8UC3, cv::Scalar(1));
  EXPECT_TRUE(!nly::is_mat_equal(mat_0, mat_1));
}

TEST(OpencvHelper, Serialize)
{
  cv::Mat                    mat_input;
  cv::Mat                    mat_output;
  std::vector<unsigned char> output;

  auto fun = [&mat_input, &mat_output, &output]()
  {
    for (int i = 0; i < 2; ++i)
    {
      nly::mat_to_data(mat_input, output);
      mat_output = nly::mat_from_data(output, i == 0);
      EXPECT_TRUE(nly::is_mat_equal(mat_input, mat_output));
    }
  };

  fun();

  std::vector<unsigned char> value{ 1, 2, 3, 0, 4, 5, 6, 0 };
  mat_input = nly::create_mat(2, 3, CV_8UC1, value.data(), 4);
  fun();

  mat_input = nly::create_mat(2, 3, CV_8UC3);
  mat_input.at<cv::Vec3b>(1, 1)[0] = 1;
  mat_input.at<cv::Vec3b>(1, 1)[1] = 2;
  mat_input.at<cv::Vec3b>(1, 1)[2] = 3;
  fun();
}

TEST(OpencvHelper, MatConcat)
{
  cv::Mat mat_0(1, 2, CV_8UC1, cv::Scalar(1));
  cv::Mat mat_1(2, 2, CV_8UC1, cv::Scalar(2));
  cv::Mat mat_2(3, 2, CV_8UC1, cv::Scalar(3));

  cv::Mat mat_4(1, 3, CV_8UC1, cv::Scalar(4));
  cv::Mat mat_5(1, 4, CV_8UC1, cv::Scalar(5));

  cv::Mat mat_6(1, 2, CV_16UC1, cv::Scalar(6));

  auto output = nly::mat_concat({ mat_0 }, true);
  checkMat(*output, 2, 1, std::vector<unsigned char>{ 1, 1 });

  output = nly::mat_concat({ mat_0, mat_1 }, true);
  checkMat(*output, 2, 3, std::vector<unsigned char>{ 1, 1, 2, 2, 2, 2 });

  output = nly::mat_concat({ mat_0, mat_1, mat_2 }, true);
  checkMat(*output, 2, 6, std::vector<unsigned char>{ 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3 });

  output = nly::mat_concat({ mat_0 }, false);
  checkMat(*output, 2, 1, std::vector<unsigned char>{ 1, 1 });

  output = nly::mat_concat({ mat_0, mat_4 }, false);
  checkMat(*output, 5, 1, std::vector<unsigned char>{ 1, 1, 4, 4, 4 });

  output = nly::mat_concat({ mat_0, mat_4, mat_5 }, false);
  checkMat(*output, 9, 1, std::vector<unsigned char>{ 1, 1, 4, 4, 4, 5, 5, 5, 5 });

  for (int i = 0; i < 2; ++i)
  {
    output = nly::mat_concat({ mat_1, mat_4 }, i == 0);
    EXPECT_TRUE(!output.has_value());

    output = nly::mat_concat({ mat_1, mat_6 }, i == 0);
    EXPECT_TRUE(!output.has_value());
  }
}

TEST(OpencvHelper, GetSpecificRegion)
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
