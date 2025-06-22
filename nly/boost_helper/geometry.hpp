#ifndef NLY_BOOST_HELPER_GEOMETRY
#define NLY_BOOST_HELPER_GEOMETRY

#define _USE_MATH_DEFINES
#include <cmath>
#include <cassert>
#include "boost/geometry.hpp"

namespace nly
{

class geometry
{
public:
  // 默认使用笛卡尔坐标系(Y 向上为正, X 向右为正), 即模板列表中的 cs::cartesian
  template<typename T>
  using point_xy_ = boost::geometry::model::d2::point_xy<T>;
  typedef point_xy_<double> point2d;
  typedef point_xy_<float>  point2f;
  typedef point_xy_<int>    point2i;

public:
  // 由两个点组成的线段
  template<typename T>
  using segment_ = boost::geometry::model::segment<T>;
  typedef segment_<point2d> segment2d;
  typedef segment_<point2f> segment2f;
  typedef segment_<point2i> segment2i;

  template<typename T>
  static segment_<point_xy_<T>> make_segment(T x0, T y0, T x1, T y1)
  {
    return segment_<point_xy_<T>>(point_xy_<T>(x0, y0), point_xy_<T>(x1, y1));
  }

public:
  // 总是与轴平行的矩形
  template<typename T>
  using rect_ = boost::geometry::model::box<T>;
  typedef rect_<point2d> rect2d;
  typedef rect_<point2f> rect2f;
  typedef rect_<point2i> rect2i;

  template<typename T>
  static rect_<point_xy_<T>> make_rect(
    T bottom_left_x,
    T bottom_left_y,
    T top_right_x,
    T top_right_y)
  {
    return rect_<point_xy_<T>>(
      point_xy_<T>(bottom_left_x, bottom_left_y),
      point_xy_<T>(top_right_x, top_right_y));
  }

  template<typename T>
  static rect_<point_xy_<T>> make_rect(const point_xy_<T>& bottom_left, T width, T height)
  {
    return rect_<point_xy_<T>>(
      bottom_left,
      point_xy_<T>(bottom_left.x() + width, bottom_left.y() + height));
  }

  template<typename T>
  static T get_rect_width(const rect_<point_xy_<T>>& rect)
  {
    return rect.max_corner().x() - rect.min_corner().x();
  }

  template<typename T>
  static T get_rect_height(const rect_<point_xy_<T>>& rect)
  {
    return rect.max_corner().y() - rect.min_corner().y();
  }

public:
  // 由一系列有序点连接而成, 首尾点不自动相连, 允许自相交
  template<typename T>
  using linestring_ = boost::geometry::model::linestring<T>;
  typedef linestring_<point2d> linestring2d;
  typedef linestring_<point2f> linestring2f;
  typedef linestring_<point2i> linestring2i;

public:
  /*
  定义了一个环
  注意:
    1. 首末点必须手动重合
    2. 实测, 若按顺时针定义, 则面积视为正, 否则面积视为负
  */
  template<typename T>
  using ring_ = boost::geometry::model::ring<T>;
  typedef ring_<point2d> ring2d;
  typedef ring_<point2f> ring2f;
  typedef ring_<point2i> ring2i;

  /*
  生成一个圆(本质是用一系列点进行模拟的)
  形参:
    center: 圆心
    radius: 半径
    sample:
      用于模拟圆的点个数, 此值越大则圆越准确, 同时计算也更慢
      经测试, 36 是一个比较均衡的值
  注意:
    1. 模板参数必须是浮点数
    2. sample 小于等于 2 是没意义的, 会返回一个空的 ring
  */
  template<typename T>
  static ring_<point_xy_<T>> make_circle(const point_xy_<T>& center, T radius, int sample = 36)
  {
    static_assert(
      std::is_floating_point_v<T>,
      "Error: nly::geometry::make_circle only accepts floating-point types.");

    if (sample <= 2)
    {
      assert(false);
      return {};
    }

    ring_<point_xy_<T>> circle;
    const double        one_part = -2 * M_PI / sample;

    for (int i = 0; i < sample; ++i)
    {
      double angle = one_part * i;
      boost::geometry::append(
        circle,
        point_xy_<T>(center.x() + radius * cos(angle), center.y() + radius * sin(angle)));
    }

    boost::geometry::append(circle, circle.front());
    return circle;
  }

  template<typename T>
  static ring_<point_xy_<T>> make_circle(T center_x, T center_y, T radius, int sample = 36)
  {
    return make_circle(point_xy_<T>(center_x, center_y), radius, sample);
  }

  // 构建一个由 ring_ 表示的矩形, 不同于 rect_, 由 ring_ 表示的矩形可以不与轴平行
  template<typename T>
  static ring_<point_xy_<T>> make_ring_rect(
    const point_xy_<T>& bottom_left,
    const point_xy_<T>& top_right)
  {
    ring_<point_xy_<T>> output;

    boost::geometry::append(output, bottom_left);
    boost::geometry::append(output, point_xy_<T>(bottom_left.x(), top_right.y()));
    boost::geometry::append(output, top_right);
    boost::geometry::append(output, point_xy_<T>(top_right.x(), bottom_left.y()));
    boost::geometry::append(output, bottom_left);

    return output;
  }

  template<typename T>
  static ring_<point_xy_<T>> make_ring_rect(
    T bottom_left_x,
    T bottom_left_y,
    T top_right_x,
    T top_right_y)
  {
    return make_ring_rect(
      point_xy_<T>(bottom_left_x, bottom_left_y),
      point_xy_<T>(top_right_x, top_right_y));
  }

  template<typename T>
  static ring_<point_xy_<T>> make_ring_rect(const point_xy_<T>& bottom_left, T width, T height)
  {
    return make_ring_rect(
      bottom_left,
      point_xy_<T>(bottom_left.x() + width, bottom_left.y() + height));
  }

  template<typename T>
  static ring_<point_xy_<T>> make_ring_rect(const rect_<T>& rect)
  {
    return make_ring_rect(rect.min_corner(), rect.max_corner());
  }

public:
  /*
  polygon_ 由一个外环和任意个内环组成
  外环:
    1. 实测需按顺时针组织
    2. 首末点需手动重合
  内环:
    1. 实测需按逆时针组织
    2. 首末点需手动重合
  注意:
    1. 在计算面积时, 内环部分将被减去
    2. 在计算周长时, 内环部分将被加上
  */
  template<typename T>
  using polygon_ = boost::geometry::model::polygon<T>;
  typedef polygon_<point2d> polygon2d;
  typedef polygon_<point2f> polygon2f;
  typedef polygon_<point2i> polygon2i;

public:
  template<typename t_geometry>
  static auto is_valid(const t_geometry& geometry)
  {
    return boost::geometry::is_valid(geometry);
  }

  /*
  修正输入的几何体, 包括:
    1. 方向与预期不符的环(包括 polygon_ 的内外环)
    2. 理应闭合却没有闭合的几何体
    3. 使用了错误 corner point 的 rect_
  */
  template<typename t_geometry>
  static void correct(t_geometry& geometry)
  {
    boost::geometry::correct(geometry);
  }

  /*
  检查输入的两个形状是否相等(即: 是否拥有相同的点集)
  注意:
    1. 不是所有的形状的组合都被实现, 具体哪些被实现了可以参考官网
    2. 对于不同的输入类型, 以下常用组合被支持
       rect_ & ring_
       rect_ & polygon_
       ring_ & polygon_
  */
  template<typename t_geometry_0, typename t_geometry_1>
  static bool equals(const t_geometry_0& geometry_0, const t_geometry_1& geometry_1)
  {
    return boost::geometry::equals(geometry_0, geometry_1);
  }

  /*
  计算连续点之间的距离和
  经测试, 若输入是以下类型, 则固定返回 0
    nly::rect_
    nly::ring_
    nly::polygon_
  */
  template<typename t_geometry>
  static auto length(const t_geometry& geometry)
  {
    return boost::geometry::length(geometry);
  }

  /*
  计算几何体的面积
  若输入是 linestring_, 则固定返回 0
  若输入的 ring_ polygon_ 不符合预期, 则面积为负
  */
  template<typename t_geometry>
  static auto area(const t_geometry& geometry)
  {
    return boost::geometry::area(geometry);
  }

  /*
  计算周长
  若输入是以下类型, 则固定返回 0
    nly::segment_
    nly::linestring_
  */
  template<typename t_geometry>
  static auto perimeter(const t_geometry& geometry)
  {
    return boost::geometry::perimeter(geometry);
  }
};

} // namespace nly

#endif // !NLY_BOOST_HELPER_GEOMETRY
