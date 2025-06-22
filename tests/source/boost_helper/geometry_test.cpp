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
