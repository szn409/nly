#ifndef NLY_GEOMETRY
#define NLY_GEOMETRY
#include "opencv2/opencv.hpp"
#include <algorithm>

namespace nly
{

/*
功能: 从 input 中抠出 region 指示的范围
形参:
  input: 输入源
  region: 目标区域
  cut:
    true: 目标区域若不在输入源的合法范围, 则直接裁切掉非法区域
    false:
      A. 目标区域若不在输入源的合法范围, 则进行平移以满足合法性
      B. 对于平移后仍不满足合法范围的部分, 则进行裁切
      C. 注意: 当 cut 为 false 时, 即使 input 和 region 完全不相交, 也会进行平移, 从而产生非空输出
返回值:
    A. 可能为空
    B. 若非空, 则为输入源的浅拷贝
*/
inline cv::Mat get_specific_region(const cv::Mat input, cv::Rect region, bool cut = true)
{
  const auto input_stop_x = input.cols - 1;
  const auto input_stop_y = input.rows - 1;

  auto x_start = region.x;
  auto x_stop = region.x + region.width - 1;
  auto y_start = region.y;
  auto y_stop = region.y + region.height - 1;

  if (x_stop < x_start || y_stop < y_start || input.empty())
  {
    return {};
  }

  if (cut)
  {
    if (x_start > input_stop_x || x_stop < 0 || y_start > input_stop_y || y_stop < 0)
    {
      return {};
    }

    x_start = std::clamp(x_start, 0, input_stop_x);
    x_stop = std::clamp(x_stop, 0, input_stop_x);
    y_start = std::clamp(y_start, 0, input_stop_y);
    y_stop = std::clamp(y_stop, 0, input_stop_y);
  }
  else
  {
    if (x_start < 0)
    {
      x_stop -= x_start;
      x_start = 0;
    }

    if (x_stop > input_stop_x)
    {
      x_start -= (x_stop - input_stop_x);
      x_stop = input_stop_x;
    }

    if (y_start < 0)
    {
      y_stop -= y_start;
      y_start = 0;
    }

    if (y_stop > input_stop_y)
    {
      y_start -= (y_stop - input_stop_y);
      y_stop = input_stop_y;
    }

    return get_specific_region(
      input,
      cv::Rect(x_start, y_start, x_stop - x_start + 1, y_stop - y_start + 1),
      true);
  }

  assert(x_start >= 0 && x_stop <= input_stop_x);
  assert(y_start >= 0 && y_stop <= input_stop_y);
  return input(cv::Rect(x_start, y_start, x_stop - x_start + 1, y_stop - y_start + 1));
}

} // namespace nly

#endif