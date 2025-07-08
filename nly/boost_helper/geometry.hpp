#ifndef NLY_BOOST_HELPER_GEOMETRY
#define NLY_BOOST_HELPER_GEOMETRY

#include <array>
#include <cassert>
#include "../math.hpp"
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
  /*
  将 input 转换为 output
  经测试, 支持的组合至少包括:
    1. segment_ to linestring_
    2. rect_ to ring_
    3. rect_ to polygon_
    4. ring_ to polygon_
    5. polygon_ to ring_, 仅保留外环, 内环将被忽略
  */
  template<typename t_geometry_input, typename t_geometry_output>
  static void assign(const t_geometry_input& input, t_geometry_output& output)
  {
    return boost::geometry::assign(output, input);
  }

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

  // 求几何体的质心
  template<typename t_geometry, typename t_point>
  static void centroid(const t_geometry& geometry, t_point& output)
  {
    return boost::geometry::centroid(geometry, output);
  }

  /*
  计算两个 geometry 的交集
  注意:
    1. 最好对 out 进行 clear, 因为不存在交集时, 此函数不会清空 out, 容易造成误解
    2. 推测: 返回 false 则认为无交集, 否则进一步判断 out 来判断是否有交集
  输入输出测试(1.87.0)
    A.
       1. point_ & segment_
          输出类型: std::vector<point_>
          边界和内部: 均包含在计算中
          返回值: 无论是否有交集, 总是返回 true
       2. point_ & linestring_: 同 point_ & segment_
       3. point_ & rect_: 同 point_ & segment_
       4. point & ring_: 同 point_ & segment_
       5. point & polygon_: 同 point_ & segment_
    B.
       1. segment_ & segment_
          输出类型: std::vector<point_>
          边界和内部: 均包含在计算中
          返回值: 无论是否有交集, 总是返回 true
       2. segment & linestring_: 不支持
       3. segment & rect_: 不支持
       4. segment & ring_: 不支持
       5. segment & polygon_: 不支持
    C.
       1. linestring_ & linestring_
          输出类型: std::vector<point_>
          边界和内部: 均包含在计算中
          返回值: 无论是否有交集, 总是返回 true
          注意: 有返回重复点的情况, 不符合预期, 具体参考测试用例
       2. linestring_ & rect_: 不支持
       3. linestring_ & ring_
          输出类型: std::vector<point_>
          边界: 包含在计算中
          内部: 位于 rect_ 内部的 linestring_ 中的点, 不会包含在返回值中
          返回值: 无论是否有交集, 总是返回 true
       4. linestring_ & polygon_: 同 linestring_ & ring_
    D.
       1. rect_ & rect_
          输出类型: rect_
          边界和内部: 均包含在计算中
          返回值: 仅在有交集时, 返回 true
       2. rect_ & ring_
          输出类型: ring_
          边界: 包含在计算中
          内部: 不会包含在返回值中
          注意: 通道对输出调用下 correct 是一个好主意
          返回值: 无论是否有交集, 总是返回 true
       3. rect_ & polygon_: 同 rect_ & ring_
    E.
       1. ring_ & ring_
          输出类型: ring_
          边界: 包含在计算中
          内部: 不会包含在返回值中
          注意: 通道对输出调用下 correct 是一个好主意
          返回值: 无论是否有交集, 总是返回 true
       2. ring_ & polygon_: 同 ring_ & ring_
    F.
       1. polygon_ & polygon_
          输出类型: polygon_
          边界和内部: 均包含在计算中
          注意: 通道对输出调用下 correct 是一个好主意
          返回值: 无论是否有交集, 总是返回 true
  */
  template<typename t_geometry_0, typename t_geometry_1, typename t_out>
  static bool intersection(
    const t_geometry_0& geometry_0,
    const t_geometry_1& geometry_1,
    t_out&              out)
  {
    return boost::geometry::intersection(geometry_0, geometry_1, out);
  }

  /*
  判断 geometry_small 是否在 geometry_big 内部
  type:
    0: 使用非零环绕规则
    1: 使用射线法(franklin)
    其他: 使用射线法(crossings_multiply), 但是优化了浮点数精度处理
  注意:
    1. 边界点不算内部(但是测试发现: polygon 的某个顶点, type 传 2, 会判断某些顶点在 polygon 内)
    2. 判断 polygon 是否在另一个 polygon 内, 此函数编译会报错, 此时使用 boost 库内无策略重载版本即可
  */
  template<typename t_geometry_small, typename t_geometry_big>
  static bool within(
    const t_geometry_small& geometry_small,
    const t_geometry_big&   geometry_big,
    int                     type = 0)
  {
    if (0 == type)
    {
      return boost::geometry::within(
        geometry_small,
        geometry_big,
        boost::geometry::strategy::within::cartesian_winding<>());
    }
    else if (1 == type)
    {
      return boost::geometry::within(
        geometry_small,
        geometry_big,
        boost::geometry::strategy::within::franklin<
          boost::geometry::point_type<decltype(geometry_small)>::type>());
    }
    else
    {
      return boost::geometry::within(
        geometry_small,
        geometry_big,
        boost::geometry::strategy::within::crossings_multiply<
          boost::geometry::point_type<decltype(geometry_small)>::type>());
    }
  }

  /*
  判断 geometry_small 是否在 geometry_big 的内部(含边界)
  此函数和 within 的不同之处:
    1. within 支持不同的判断策略, 比如支持使用非零缠绕
    2. within 判断时不含边界, 但此函数支持 geometry_small 在 geometry_big 边界上
  */
  template<typename t_geometry_small, typename t_geometry_big>
  static bool covered_by(const t_geometry_small& geometry_small, const t_geometry_big& geometry_big)
  {
    return boost::geometry::covered_by(geometry_small, geometry_big);
  }

  /*
  若两个 geometry 不相交, 则返回 true
  经测试, 边界纳入计算, 即:
    1. 若两个点重合, 此函数返回 false
    2. 若点在 rect 的边上, 此函数返回 false
    3. 若两个 rect 相邻, 此函数返回 false
  */
  template<typename t_geometry_0, typename t_geometry_1>
  static bool disjoint(const t_geometry_0& geometry_0, const t_geometry_1& geometry_1)
  {
    return boost::geometry::disjoint(geometry_0, geometry_1);
  }

  /*
  计算两个几何体之间的最近点
  经测试: 若两个几何体存在重合, 则返回的线段的首末点是相等的
  */
  template<typename t_geometry_0, typename t_geometry_1, typename t_segment>
  static void closest_points(
    const t_geometry_0& geometry_0,
    const t_geometry_1& geometry_1,
    t_segment&          output)
  {
    boost::geometry::closest_points(geometry_0, geometry_1, output);
  }

  // 计算 input 的凸包
  template<typename t_geometry, typename t_output>
  static void convex_hull(const t_geometry& input, t_output& output)
  {
    boost::geometry::convex_hull(input, output);
  }

  /*
  功能: 获取绕指定点逆时针旋转的矩阵
  形参:
    center: 旋转中心
    angle_rad: 旋转的弧度值
  过程:
    1. 先将旋转中心(x, y)平移到原点, 矩阵(T0)为
       1 0 -x
       0 1 -y
       0 0 1
    2. 绕原点旋转(逆时针), 矩阵(T1)为
       cosθ -sinθ 0
       sinθ cosθ  0
       0    0     1
    3. 平移回原位置, 矩阵(T2)为
       1 0 x
       0 1 y
       0 0 1
    4. 记输入点坐标 P 为
       Xp
       Yp
       1
       则最终坐标为: T2 * T1 * T0 * P
  输出: [a, b, c, d, tx, ty], 含义:
    a c tx
    b d ty
    0 0 1
  */
  static std::array<double, 6> get_rotate_matrix(const point2d& center, double angle_rad)
  {
    double cos_a = std::cos(angle_rad);
    double sin_a = std::sin(angle_rad);

    return std::array<double, 6>{
      cos_a,
      sin_a,
      -sin_a,
      cos_a,
      center.x() * (1 - cos_a) + center.y() * sin_a,
      center.y() * (1 - cos_a) - center.x() * sin_a,
    };
  }

  // 用户一般无需直接调用此函数
  template<typename t_geometry_input, typename t_geometry_output, typename t_strategy>
  static bool transform(
    const t_geometry_input& input,
    t_geometry_output&      output,
    const t_strategy&       strategy)
  {
    return boost::geometry::transform(input, output, strategy);
  }

  /*
  1. 将 input 进行变换, 输入的矩阵是 [a, b, c, d, tx, ty]
  2. 若需要同时进行旋转、缩放、平移, 则调用此函数效率比较高, 缺点是需要自行计算矩阵
  3. 自行计算矩阵时的过程, 可以参考 get_rotate_matrix
  */
  template<typename t_geometry_input, typename t_geometry_output>
  static bool transform(
    const t_geometry_input& input,
    t_geometry_output&      output,
    double                  a,
    double                  b,
    double                  c,
    double                  d,
    double                  tx,
    double                  ty)
  {
    return transform(
      input,
      output,
      boost::geometry::strategy::transform::
        matrix_transformer<double, 2, 2>(a, c, tx, b, d, ty, 0, 0, 1));
  }

  template<typename t_geometry_input, typename t_geometry_output>
  static bool move(
    const t_geometry_input& input,
    t_geometry_output&      output,
    double                  xTranslate,
    double                  yTranslate)
  {
    return transform(
      input,
      output,
      boost::geometry::strategy::transform::translate_transformer<double, 2, 2>(
        xTranslate,
        yTranslate));
  }

  // 将 input 以 center 为中心, 逆时针旋转 angle_rad(单位: 弧度)
  template<typename t_geometry_input, typename t_geometry_output>
  static bool rotate(
    const t_geometry_input& input,
    t_geometry_output&      output,
    double                  angle_rad,
    point2d                 center = point2d{ 0.0, 0.0 })
  {
    auto [a, b, c, d, tx, ty] = get_rotate_matrix(center, angle_rad);
    return transform(input, output, a, b, c, d, tx, ty);
  }

  // 注意: 当缩放系数为负时, 会引入镜像翻转
  template<typename t_geometry_input, typename t_geometry_output>
  static bool scale(
    const t_geometry_input& input,
    t_geometry_output&      output,
    double                  x_scale,
    double                  y_scale)
  {
    return transform(
      input,
      output,
      boost::geometry::strategy::transform::scale_transformer<double, 2, 2>(x_scale, y_scale));
  }
};

} // namespace nly

#endif // !NLY_BOOST_HELPER_GEOMETRY
