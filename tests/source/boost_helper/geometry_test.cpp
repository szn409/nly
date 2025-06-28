#include "gtest/gtest.h"
#include "nly/boost_helper/geometry.hpp"
#include "nly/math.hpp"

TEST(Geometry, Point)
{
  nly::geometry::point2d p0(1.2, 3.4);
  EXPECT_TRUE(nly::math::float_equal(p0.x(), 1.2) && nly::math::float_equal(p0.y(), 3.4));
  EXPECT_TRUE(nly::geometry::is_valid(p0));

  p0.x(4.5);
  p0.y(5.6);
  EXPECT_TRUE(nly::math::float_equal(p0.x(), 4.5) && nly::math::float_equal(p0.y(), 5.6));

  nly::geometry::point2i p1(1.2, 3.4);
  EXPECT_TRUE(p1.x() == 1 && p1.y() == 3);
}

TEST(Geometry, Segment)
{
  nly::geometry::segment2i segment0(nly::geometry::point2i(1, 2), nly::geometry::point2i(3, 4));
  auto                     segment1 = nly::geometry::make_segment(1, 2, 3, 4);
  EXPECT_TRUE(nly::geometry::equals(segment0, segment1));

  nly::geometry::segment2d segmen2(
    nly::geometry::point2d(1.1, 2.2),
    nly::geometry::point2d(3.3, 4.4));
  auto segmen3 = nly::geometry::make_segment(3.3, 4.4, 1.1, 2.2);
  auto segmen4 = nly::geometry::make_segment(1.1, 2.2, 3.3, 4.4);
  EXPECT_TRUE(nly::geometry::equals(segmen2, segmen3));
  EXPECT_TRUE(nly::geometry::equals(segmen2, segmen4));
  EXPECT_TRUE(nly::geometry::is_valid(segmen2));
  EXPECT_TRUE(nly::math::float_equal(nly::geometry::length(segment0), std::sqrt(8)));
}

TEST(Geometry, Rect)
{
  nly::geometry::rect2i rect_0(nly::geometry::point2i(1, 1), nly::geometry::point2i(3, 4));
  auto                  rect_1 = nly::geometry::make_rect(nly::geometry::point2i(1, 1), 2, 3);
  auto                  rect_2 = nly::geometry::make_rect(1, 1, 3, 4);
  EXPECT_TRUE(nly::geometry::equals(rect_0, rect_1));
  EXPECT_TRUE(nly::geometry::equals(rect_0, rect_2));
  EXPECT_TRUE(nly::geometry::is_valid(rect_0));
  EXPECT_TRUE(nly::math::float_equal(nly::geometry::area(rect_0), 6));
  EXPECT_TRUE(nly::math::float_equal(nly::geometry::get_rect_width(rect_0), 2));
  EXPECT_TRUE(nly::math::float_equal(nly::geometry::get_rect_height(rect_0), 3));
  EXPECT_TRUE(nly::math::float_equal(nly::geometry::get_rect_width(nly::geometry::rect2d()), 0));
  EXPECT_TRUE(nly::math::float_equal(nly::geometry::get_rect_height(nly::geometry::rect2d()), 0));

  nly::geometry::rect2i rect_3(nly::geometry::point2i(3, 4), nly::geometry::point2i(1, 1));
  EXPECT_TRUE(!nly::geometry::is_valid(rect_3));
  EXPECT_TRUE(!nly::geometry::equals(rect_0, rect_3));
  nly::geometry::correct(rect_3);
  EXPECT_TRUE(nly::geometry::is_valid(rect_3));
  EXPECT_TRUE(nly::geometry::equals(rect_0, rect_3));
}

TEST(Geometry, LineString)
{
  nly::geometry::linestring2i line;
  line.emplace_back(nly::geometry::point2i(0, 0));
  line.emplace_back(nly::geometry::point2i(1, 1));
  line.emplace_back(nly::geometry::point2i(2, 0));
  EXPECT_TRUE(nly::geometry::is_valid(line));
  EXPECT_TRUE(nly::math::float_equal(nly::geometry::length(line), sqrt(2) + sqrt(2)));
}

TEST(Geometry, Ring)
{
  nly::geometry::ring2i ring;
  boost::geometry::append(ring, nly::geometry::point2i(0, 0));
  boost::geometry::append(ring, nly::geometry::point2i(0, 4));
  boost::geometry::append(ring, nly::geometry::point2i(4, 4));
  boost::geometry::append(ring, nly::geometry::point2i(4, 0));

  EXPECT_TRUE(!nly::geometry::is_valid(ring));
  boost::geometry::append(ring, nly::geometry::point2i(0, 0));
  EXPECT_TRUE(16 == nly::geometry::area(ring));

  ring.clear();
  boost::geometry::append(ring, nly::geometry::point2i(0, 0));
  boost::geometry::append(ring, nly::geometry::point2i(4, 0));
  boost::geometry::append(ring, nly::geometry::point2i(4, 4));
  boost::geometry::append(ring, nly::geometry::point2i(0, 4));
  boost::geometry::append(ring, nly::geometry::point2i(0, 0));
  EXPECT_TRUE(-16 == nly::geometry::area(ring));
  EXPECT_TRUE(!nly::geometry::is_valid(ring));

  nly::geometry::correct(ring);
  EXPECT_TRUE(16 == nly::geometry::area(ring));
  EXPECT_TRUE(nly::geometry::is_valid(ring));
  EXPECT_TRUE(16 == nly::geometry::perimeter(ring));
}

TEST(Geometry, Circle)
{
  auto circle = nly::geometry::make_circle(2.0, 3.0, 10.0);

  double sampleArea = nly::geometry::area(circle);
  double realArea = M_PI * pow(10, 2);
  EXPECT_TRUE(sampleArea < realArea);
  EXPECT_TRUE(std::abs(sampleArea - realArea) / realArea < 0.01);

  double samplePerimeter = nly::geometry::perimeter(circle);
  double realPerimeter = 2 * M_PI * 10;
  EXPECT_TRUE(samplePerimeter < realPerimeter);
  EXPECT_TRUE(std::abs(samplePerimeter - realPerimeter) / realPerimeter < 0.01);
}

TEST(Geometry, MakeRingRect)
{
  auto ring_rect_0 = nly::geometry::make_ring_rect(1, 2, 3, 5);
  auto ring_rect_1 = nly::geometry::make_ring_rect(nly::geometry::point2i(1, 2), 2, 3);
  auto ring_rect_2 = nly::geometry::make_rect(nly::geometry::point2i(1, 2), 2, 3);
  EXPECT_TRUE(nly::geometry::equals(ring_rect_0, ring_rect_1));
  EXPECT_TRUE(nly::geometry::equals(ring_rect_0, ring_rect_2));
  EXPECT_TRUE(nly::geometry::is_valid(ring_rect_0));
  EXPECT_TRUE(nly::math::float_equal(nly::geometry::area(ring_rect_0), 6));
  EXPECT_TRUE(nly::math::float_equal(nly::geometry::perimeter(ring_rect_0), 10));
}

TEST(Geometry, Polygon)
{
  nly::geometry::polygon2i poly;
  boost::geometry::append(poly.outer(), nly::geometry::point2i(0, 0));
  boost::geometry::append(poly.outer(), nly::geometry::point2i(0, 4));
  boost::geometry::append(poly.outer(), nly::geometry::point2i(4, 4));
  boost::geometry::append(poly.outer(), nly::geometry::point2i(4, 0));
  EXPECT_TRUE(16 == nly::geometry::area(poly));

  EXPECT_TRUE(!nly::geometry::is_valid(poly));
  boost::geometry::append(poly.outer(), nly::geometry::point2i(0, 0));
  EXPECT_TRUE(nly::geometry::is_valid(poly));
  EXPECT_TRUE(16 == nly::geometry::perimeter(poly));
  EXPECT_TRUE(16 == nly::geometry::area(poly));

  poly.clear();
  boost::geometry::append(poly.outer(), nly::geometry::point2i(0, 0));
  boost::geometry::append(poly.outer(), nly::geometry::point2i(5, 0));
  boost::geometry::append(poly.outer(), nly::geometry::point2i(5, 5));
  boost::geometry::append(poly.outer(), nly::geometry::point2i(0, 5));
  boost::geometry::append(poly.outer(), nly::geometry::point2i(0, 0));

  EXPECT_TRUE(-25 == nly::geometry::area(poly));
  EXPECT_TRUE(!nly::geometry::is_valid(poly));

  nly::geometry::correct(poly);
  EXPECT_TRUE(nly::geometry::is_valid(poly));
  EXPECT_TRUE(25 == nly::geometry::area(poly));

  auto& inner_rings = poly.inners();
  inner_rings.resize(1);

  inner_rings[0].emplace_back(nly::geometry::point2i(1, 1));
  inner_rings[0].emplace_back(nly::geometry::point2i(2, 1));
  inner_rings[0].emplace_back(nly::geometry::point2i(2, 2));
  inner_rings[0].emplace_back(nly::geometry::point2i(1, 2));
  inner_rings[0].emplace_back(nly::geometry::point2i(1, 1));
  EXPECT_TRUE(24 == nly::geometry::area(poly));
  EXPECT_TRUE(nly::geometry::is_valid(poly));

  inner_rings.resize(2);
  inner_rings[1].emplace_back(nly::geometry::point2i(3, 3));
  inner_rings[1].emplace_back(nly::geometry::point2i(3, 4));
  inner_rings[1].emplace_back(nly::geometry::point2i(4, 4));
  inner_rings[1].emplace_back(nly::geometry::point2i(4, 3));
  inner_rings[1].emplace_back(nly::geometry::point2i(3, 3));
  EXPECT_TRUE(25 == nly::geometry::area(poly));
  EXPECT_TRUE(!nly::geometry::is_valid(poly));

  nly::geometry::correct(poly);
  EXPECT_TRUE(nly::geometry::is_valid(poly));
  EXPECT_TRUE(23 == nly::geometry::area(poly));
  EXPECT_TRUE(28 == nly::geometry::perimeter(poly));
}

template<typename t_point, typename t_geometry_small, typename t_geometry_big>
void check_winthin(
  const t_geometry_small& g_small,
  const t_geometry_big&   g_big,
  bool                    for_default,
  bool                    for_winding,
  bool                    for_franklin,
  bool                    for_crossings_multiply)
{
  EXPECT_TRUE(nly::geometry::within(g_small, g_big) == for_default);
  EXPECT_TRUE(nly::geometry::within(g_small, g_big, 0) == for_winding);
  EXPECT_TRUE(nly::geometry::within(g_small, g_big, 1) == for_franklin);
  EXPECT_TRUE(nly::geometry::within(g_small, g_big, 2) == for_crossings_multiply);
}

TEST(Geometry, Within)
{
  {
    nly::geometry::polygon2i star;
    star.outer().emplace_back(0, 100);
    star.outer().emplace_back(-58, -81);
    star.outer().emplace_back(95, 31);
    star.outer().emplace_back(-95, 31);
    star.outer().emplace_back(58, -81);
    star.outer().emplace_back(0, 100);
    nly::geometry::correct(star);
    check_winthin<nly::geometry::point2i>(nly::geometry::point2i{}, star, true, true, false, false);
    check_winthin<nly::geometry::point2i>(
      nly::geometry::point2i{ 1000, 1000 },
      star,
      false,
      false,
      false,
      false);
    check_winthin<nly::geometry::point2i>(
      nly::geometry::point2i{ 0, 100 },
      star,
      false,
      false,
      false,
      true);
    check_winthin<nly::geometry::point2i>(
      nly::geometry::point2i{ -58, -81 },
      star,
      false,
      false,
      false,
      false);
    check_winthin<nly::geometry::point2i>(
      nly::geometry::point2i{ 95, 31 },
      star,
      false,
      false,
      false,
      false);
    check_winthin<nly::geometry::point2i>(
      nly::geometry::point2i{ -95, 31 },
      star,
      false,
      false,
      false,
      false);
    check_winthin<nly::geometry::point2i>(
      nly::geometry::point2i{ 58, -81 },
      star,
      false,
      false,
      false,
      false);
  }

  {
    auto rect = nly::geometry::make_rect(-100, -100, 100, 100);
    check_winthin<nly::geometry::point2i>(nly::geometry::point2i{}, rect, true, true, true, true);
    check_winthin<nly::geometry::point2i>(
      nly::geometry::point2i{ 1000, 1000 },
      rect,
      false,
      false,
      false,
      false);
    check_winthin<nly::geometry::point2i>(
      nly::geometry::point2i{ -100, 0 },
      rect,
      false,
      false,
      false,
      false);
    check_winthin<nly::geometry::point2i>(
      nly::geometry::point2i{ -100, -100 },
      rect,
      false,
      false,
      false,
      false);
    check_winthin<nly::geometry::point2i>(
      nly::geometry::point2i{ 100, 100 },
      rect,
      false,
      false,
      false,
      false);

    check_winthin<nly::geometry::point2i>(rect, rect, true, true, true, true);
  }

  {
    auto rect_0 = nly::geometry::make_rect(-100, -100, 50, 50);
    auto rect_1 = nly::geometry::make_rect(-100, -100, 100, 100);
    check_winthin<nly::geometry::point2i>(rect_0, rect_1, true, true, true, true);
  }
}

TEST(Geometry, TransformRotate)
{
  {
    auto check = [](double angle_rad)
    {
      nly::geometry::point2d output;
      EXPECT_TRUE(nly::geometry::rotate(nly::geometry::point2d(1, 0), output, angle_rad));
      EXPECT_TRUE(nly::math::float_equal(output.x(), std::cos(angle_rad)));
      EXPECT_TRUE(nly::math::float_equal(output.y(), std::sin(angle_rad)));
    };
    for (int i = 0; i < 1000; i += 15)
    {
      check(nly::math::to_rad(i));
    }
  }

  {
    nly::geometry::point2d output;
    EXPECT_TRUE(
      nly::geometry::rotate(
        nly::geometry::point2d(10, 20),
        output,
        nly::math::to_rad(30),
        nly::geometry::point2d(1, 2)));
    EXPECT_TRUE(nly::math::float_equal(output.x(), -0.206, 0.001));
    EXPECT_TRUE(nly::math::float_equal(output.y(), 22.088, 0.001));
  }

  {
    nly::geometry::point2d output;
    EXPECT_TRUE(
      nly::geometry::rotate(
        nly::geometry::point2d(10, 20),
        output,
        nly::math::to_rad(90),
        nly::geometry::point2d(10, 15)));
    EXPECT_TRUE(nly::math::float_equal(output.x(), 5));
    EXPECT_TRUE(nly::math::float_equal(output.y(), 15));
  }

  {
    nly::geometry::ring2d ring;
    ring.emplace_back(10, 10);
    ring.emplace_back(10, 20);
    ring.emplace_back(20, 20);
    ring.emplace_back(20, 10);
    nly::geometry::correct(ring);

    nly::geometry::ring2d output;
    nly::geometry::rotate(ring, output, nly::math::to_rad(45), nly::geometry::point2d(15, 15));

    auto check = [&output](int index, double x, double y)
    {
      auto diff = 0.0000001;
      EXPECT_TRUE(nly::math::float_equal(output.at(index).x(), x, diff));
      EXPECT_TRUE(nly::math::float_equal(output.at(index).y(), y, diff));
    };

    int index = 0;
    check(index++, 14.99999991, 7.928932188);
    check(index++, 7.928932188, 15.00000009);
    check(index++, 15.00000009, 22.07106781);
    check(index++, 22.07106781, 14.99999991);
    check(index++, 14.99999991, 7.928932188);
  }
}

TEST(Geometry, TransformMove)
{
  {
    nly::geometry::point2i output;
    EXPECT_TRUE(nly::geometry::move(nly::geometry::point2i(10, 20), output, -10, -20));
    EXPECT_TRUE(nly::math::float_equal(output.x(), 0));
    EXPECT_TRUE(nly::math::float_equal(output.y(), 0));

    EXPECT_TRUE(nly::geometry::move(nly::geometry::point2d(10, 20), output, 1, 2));
    EXPECT_TRUE(nly::math::float_equal(output.x(), 11));
    EXPECT_TRUE(nly::math::float_equal(output.y(), 22));
  }

  {
    nly::geometry::rect2d output;
    EXPECT_TRUE(nly::geometry::move(nly::geometry::make_rect(10, 20, 100, 110), output, -10, 20));
    EXPECT_TRUE(nly::geometry::equals(output, nly::geometry::make_rect(0, 40, 90, 130)));
  }
}

TEST(Geometry, TransformScale)
{
  {
    nly::geometry::point2i output;
    EXPECT_TRUE(nly::geometry::scale(nly::geometry::point2i(10, 20), output, 1.5, 2));
    EXPECT_TRUE(nly::math::float_equal(output.x(), 15));
    EXPECT_TRUE(nly::math::float_equal(output.y(), 40));

    EXPECT_TRUE(nly::geometry::scale(nly::geometry::point2d(10, 20), output, -1.5, 2));
    EXPECT_TRUE(nly::math::float_equal(output.x(), -15));
    EXPECT_TRUE(nly::math::float_equal(output.y(), 40));
  }

  {
    nly::geometry::rect2d output;
    EXPECT_TRUE(nly::geometry::scale(nly::geometry::make_rect(10, 20, 100, 110), output, 2, 3));
    EXPECT_TRUE(nly::geometry::equals(output, nly::geometry::make_rect(20, 60, 200, 330)));

    EXPECT_TRUE(nly::geometry::scale(nly::geometry::make_rect(10, 20, 100, 110), output, -2, 3));
    EXPECT_TRUE(nly::geometry::equals(output, nly::geometry::make_rect(-200, 60, -20, 330)));
  }
}
