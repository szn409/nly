#ifndef NLY_OPENCV_HELPER
#define NLY_OPENCV_HELPER
#include "opencv2/opencv.hpp"
#include <algorithm>
#include <optional>

namespace nly
{

/*
功能: 构造一个 Mat, 指定其 size 和 type 以及初始值
形参:
  height: 高度, 即 rows
  width: 宽度, 即 cols
  type:
    A. 使用 CV_8UC1, ..., CV_64FC4 来创建 1-4 channel 的 matrix
    B. 使用 CV_8UC(n), ..., CV_64FC(n) 来创建 multi-channel 的 matrix, channel 个数最大为 CV_CN_MAX
    C. 格式: CV_<bit-depth>{U|S|F}C(<number_of_channels>)
      1. bit-depth: 比特数
      2. U: 无符号数
      3. S: 有符号数
      4. F: 浮点数
      5. number_of_channels: 通道数
  s: 每个元素的初始值
*/
inline cv::Mat create_mat(int height, int width, int type, const cv::Scalar& s = {})
{
  return cv::Mat(height, width, type, s);
}

/*
功能: 以指定的数据构造 mat
形参:
  data: 所引用的外部数据, 必须自己管理好其生命期
  step:
    1. 矩阵每行占用的字节数
    2. 若每行结尾有填充字符的话，这个字段就非常有意义了
    3. 若使用默认参数 AUTO_STEP，则每行所占用的字节数为：cols * elemSize()
  其他: 参考上文此函数的重载
注意: 以这种方式构造的 Mat 不会分配和拷贝内存
*/
inline cv::Mat create_mat(
  int    height,
  int    width,
  int    type,
  void*  data,
  size_t step = cv::Mat::AUTO_STEP)
{
  return cv::Mat(height, width, type, data, step);
}

/*
功能: 判断两个 Mat 是否相等
注意: 不适用 二维 以上的场景
两个 Mat 的相等条件如下:
  1. size 必须相等
  2. type 必须相等
  3. 逐元素值比较必须相等(实际上是进行 memcmp)
*/
inline bool is_mat_equal(const cv::Mat& mat_0, const cv::Mat& mat_1)
{
  if (mat_0.dims > 2 || mat_1.dims > 2)
  {
    throw std::exception("Manipulated Mat dimensions must not exceed two.");
  }

  int height = mat_0.rows;
  int width = mat_0.cols;
  int type = mat_0.type();

  if (height != mat_1.rows || width != mat_1.cols || type != mat_1.type())
  {
    return false;
  }

  if (mat_0.empty() && mat_1.empty())
  {
    return true;
  }

  const auto per_line_byte = width * mat_0.elemSize();

  if (mat_0.isContinuous() && mat_1.isContinuous())
  {
    return memcmp(mat_0.data, mat_1.data, per_line_byte * mat_0.rows) == 0;
  }

  for (int h = 0; h < height; ++h)
  {
    if (memcmp(mat_0.ptr(h, 0), mat_1.ptr(h, 0), per_line_byte))
    {
      return false;
    }
  }

  return true;
}

// 将 mat 进行序列化
// 注意: 不适用 二维 以上的场景
inline void mat_to_data(const cv::Mat& input_mat, std::vector<unsigned char>& output_data)
{
  if (input_mat.dims > 2)
  {
    throw std::exception("Manipulated Mat dimensions must not exceed two.");
  }

  const auto per_line_byte = input_mat.empty() ? 0 : input_mat.cols * input_mat.elemSize();

  const auto byte_of_int = sizeof(int);

  // 前三个 int: 高 + 宽 + type
  output_data.resize(per_line_byte * input_mat.rows + byte_of_int * 3);

  auto data = output_data.data();

  memcpy(data, &input_mat.rows, byte_of_int);
  data += byte_of_int;

  memcpy(data, &input_mat.cols, byte_of_int);
  data += byte_of_int;

  const int type = input_mat.type();
  memcpy(data, &type, byte_of_int);
  data += byte_of_int;

  if (input_mat.empty())
  {
    return;
  }

  if (input_mat.isContinuous())
  {
    memcpy(data, input_mat.data, per_line_byte * input_mat.rows);
  }
  else
  {
    for (int h = 0; h < input_mat.rows; ++h)
    {
      memcpy(data, input_mat.ptr(h, 0), per_line_byte);
      data += per_line_byte;
    }
  }
}

/*
功能: 从数据反序列化, 得到 mat
形参:
  data:
    1. 数据源
    2. 此函数保证不修改 data, 但是当 deep_copy 为 false 时, 用户可以通过返回的 Mat 间接的修改 data
  len: data 中的有效字节数
  deep_copy:
    true: 最终返回的 Mat 将 clone 一次, 使得其底层数据与输入的 data 解耦
    false: 最终返回的 Mat 的底层数据引用 data
注意: 应和 mat_to_data 搭配使用
*/
inline cv::Mat mat_from_data(const void* data, size_t len, bool deep_copy = true)
{
  auto input_data = static_cast<const unsigned char*>(data);

  int height = 0;
  int width = 0;
  int type = 0;

  const auto byte_of_int = sizeof(int);

  memcpy(&height, input_data, byte_of_int);
  input_data += byte_of_int;

  memcpy(&width, input_data, byte_of_int);
  input_data += byte_of_int;

  memcpy(&type, input_data, byte_of_int);
  input_data += byte_of_int;

  auto output = cv::Mat(height, width, type, const_cast<unsigned char*>(input_data));
  if (deep_copy)
  {
    return output.clone();
  }

  return output;
}

inline cv::Mat mat_from_data(const std::vector<unsigned char>& data, bool deep_copy = true)
{
  return mat_from_data(data.data(), data.size(), deep_copy);
}

/*
功能: 将输入的 Mat 进行水平或垂直拼接
注意:
  1. 进行水平拼接时, 两个 Mat 必须具备相同的行数
  2. 进行垂直拼接时, 两个 Mat 必须具备相同的列数
  3. 注意: 无论是水平拼接还是垂直拼接, 输入的 Mat 的 type 必须相等
*/
inline std::optional<cv::Mat> mat_concat(std::initializer_list<cv::Mat> input, bool vertical)
{
  try
  {
    cv::Mat output;

    if (vertical)
    {
      cv::vconcat(input.begin(), input.size(), output);
      return output;
    }
    else
    {
      cv::hconcat(input.begin(), input.size(), output);
      return output;
    }
  }
  catch (...)
  {
  }

  return {};
}

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