#include "gtest/gtest.h"
#include "nly/boost_helper/geometry.hpp"
#include "nly/math.hpp"

auto make_polygon_rect(int x0, int y0, int x1, int y1)
{
  nly::geometry::polygon2i polygon;
  polygon.outer().emplace_back(x0, y0);
  polygon.outer().emplace_back(x0, y1);
  polygon.outer().emplace_back(x1, y1);
  polygon.outer().emplace_back(x1, y0);
  nly::geometry::correct(polygon);
  EXPECT_TRUE(nly::geometry::is_valid(polygon));
  return polygon;
};

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

TEST(Geometry, Assign)
{
  // segment_ to linestring_
  {
    nly::geometry::linestring2i output;
    nly::geometry::assign(nly::geometry::make_segment(0, 0, 10, 10), output);
    EXPECT_TRUE(output.size() == 2);
    EXPECT_TRUE(nly::geometry::equals(output.at(0), nly::geometry::point2i(0, 0)));
    EXPECT_TRUE(nly::geometry::equals(output.at(1), nly::geometry::point2i(10, 10)));
  }

  // rect_ to ring_
  {
    nly::geometry::ring2i output;
    nly::geometry::assign(nly::geometry::make_rect(0, 0, 10, 10), output);
    EXPECT_TRUE(nly::geometry::equals(output, nly::geometry::make_ring_rect(0, 0, 10, 10)));
  }

  // rect_ to polygon_
  {
    nly::geometry::polygon2i output;
    nly::geometry::assign(nly::geometry::make_rect(0, 0, 10, 10), output);
    EXPECT_TRUE(nly::geometry::equals(output, make_polygon_rect(0, 0, 10, 10)));
  }

  // ring_ to polygon_
  {
    nly::geometry::polygon2i output;
    nly::geometry::assign(nly::geometry::make_ring_rect(0, 0, 10, 10), output);
    EXPECT_TRUE(nly::geometry::equals(output, make_polygon_rect(0, 0, 10, 10)));
  }

  // polygon_ to ring_
  {
    auto rect = make_polygon_rect(0, 0, 100, 100);
    rect.inners().resize(1);
    auto& inner = rect.inners().front();
    inner.emplace_back(5, 5);
    inner.emplace_back(5, 95);
    inner.emplace_back(95, 95);
    inner.emplace_back(95, 5);
    nly::geometry::correct(rect);

    nly::geometry::ring2i output;
    nly::geometry::assign(rect, output);
    EXPECT_TRUE(nly::geometry::equals(output, nly::geometry::make_ring_rect(0, 0, 100, 100)));
  }
}

TEST(Geometry, Centroid)
{
  nly::geometry::point2d point;
  auto circle = nly::geometry::make_circle(nly::geometry::point2d(10.0, 20.0), 30.0);
  nly::geometry::centroid(circle, point);
  EXPECT_TRUE(nly::math::float_equal(point.x(), 10.0, 1e-6));
  EXPECT_TRUE(nly::math::float_equal(point.y(), 20.0, 1e-6));
}

TEST(Geometry, Intersection)
{
  // point & segment
  {
    nly::geometry::point2i              point(10, 20);
    std::vector<nly::geometry::point2d> output;

    auto segment = nly::geometry::make_segment(0, 20, 100, 20);
    EXPECT_TRUE(nly::geometry::intersection(point, segment, output));
    EXPECT_TRUE(output.size() == 1 && nly::geometry::equals(point, output.front()));

    output.clear();
    nly::geometry::make_segment(10, 20, 100, 20);
    EXPECT_TRUE(nly::geometry::intersection(point, segment, output));
    EXPECT_TRUE(output.size() == 1 && nly::geometry::equals(point, output.front()));

    output.clear();
    segment = nly::geometry::make_segment(0, 0, 100, 0);
    EXPECT_TRUE(nly::geometry::intersection(point, segment, output));
    EXPECT_TRUE(output.empty());
  }

  // point_ & linestring_
  {
    nly::geometry::point2i              point(10, 20);
    nly::geometry::linestring2i         linestring;
    std::vector<nly::geometry::point2d> output;

    EXPECT_TRUE(nly::geometry::intersection(point, linestring, output) && output.empty());

    output.clear();
    linestring.emplace_back(0, 20);
    linestring.emplace_back(9, 20);
    EXPECT_TRUE(nly::geometry::intersection(point, linestring, output) && output.empty());

    output.clear();
    linestring.emplace_back(10, 20);
    EXPECT_TRUE(nly::geometry::intersection(point, linestring, output));
    EXPECT_TRUE(output.size() == 1 && nly::geometry::equals(point, output.front()));

    output.clear();
    linestring.pop_back();
    linestring.emplace_back(20, 20);
    EXPECT_TRUE(nly::geometry::intersection(point, linestring, output));
    EXPECT_TRUE(output.size() == 1 && nly::geometry::equals(point, output.front()));
  }

  // point_ & rect_
  {
    nly::geometry::point2i              point(10, 20);
    std::vector<nly::geometry::point2d> output;

    auto rect = nly::geometry::make_rect(0, 0, 5, 5);
    output.clear();
    EXPECT_TRUE(nly::geometry::intersection(point, rect, output) && output.empty());

    output.clear();
    rect = nly::geometry::make_rect(0, 0, 10, 20);
    EXPECT_TRUE(nly::geometry::intersection(point, rect, output));
    EXPECT_TRUE(output.size() == 1 && nly::geometry::equals(point, output.front()));

    output.clear();
    rect = nly::geometry::make_rect(0, 0, 10, 100);
    EXPECT_TRUE(nly::geometry::intersection(point, rect, output));
    EXPECT_TRUE(output.size() == 1 && nly::geometry::equals(point, output.front()));

    output.clear();
    rect = nly::geometry::make_rect(10, 20, 100, 100);
    EXPECT_TRUE(nly::geometry::intersection(point, rect, output));
    EXPECT_TRUE(output.size() == 1 && nly::geometry::equals(point, output.front()));

    output.clear();
    rect = nly::geometry::make_rect(0, 0, 100, 100);
    EXPECT_TRUE(nly::geometry::intersection(point, rect, output));
    EXPECT_TRUE(output.size() == 1 && nly::geometry::equals(point, output.front()));
  }

  // point_ & ring_
  {
    nly::geometry::point2i              point(10, 20);
    std::vector<nly::geometry::point2d> output;

    auto rect = nly::geometry::make_ring_rect(0, 0, 5, 5);
    output.clear();
    EXPECT_TRUE(nly::geometry::intersection(point, rect, output) && output.empty());

    output.clear();
    rect = nly::geometry::make_ring_rect(0, 0, 10, 20);
    EXPECT_TRUE(nly::geometry::intersection(point, rect, output));
    EXPECT_TRUE(output.size() == 1 && nly::geometry::equals(point, output.front()));

    output.clear();
    rect = nly::geometry::make_ring_rect(0, 0, 10, 100);
    EXPECT_TRUE(nly::geometry::intersection(point, rect, output));
    EXPECT_TRUE(output.size() == 1 && nly::geometry::equals(point, output.front()));

    output.clear();
    rect = nly::geometry::make_ring_rect(10, 20, 100, 100);
    EXPECT_TRUE(nly::geometry::intersection(point, rect, output));
    EXPECT_TRUE(output.size() == 1 && nly::geometry::equals(point, output.front()));

    output.clear();
    rect = nly::geometry::make_ring_rect(0, 0, 100, 100);
    EXPECT_TRUE(nly::geometry::intersection(point, rect, output));
    EXPECT_TRUE(output.size() == 1 && nly::geometry::equals(point, output.front()));
  }

  // point_ & polygon_
  {
    nly::geometry::point2i              point(10, 20);
    std::vector<nly::geometry::point2d> output;

    auto rect = make_polygon_rect(0, 0, 5, 5);
    output.clear();
    EXPECT_TRUE(nly::geometry::intersection(point, rect, output) && output.empty());

    output.clear();
    rect = make_polygon_rect(0, 0, 10, 20);
    EXPECT_TRUE(nly::geometry::intersection(point, rect, output));
    EXPECT_TRUE(output.size() == 1 && nly::geometry::equals(point, output.front()));

    output.clear();
    rect = make_polygon_rect(0, 0, 10, 100);
    EXPECT_TRUE(nly::geometry::intersection(point, rect, output));
    EXPECT_TRUE(output.size() == 1 && nly::geometry::equals(point, output.front()));

    output.clear();
    rect = make_polygon_rect(10, 20, 100, 100);
    EXPECT_TRUE(nly::geometry::intersection(point, rect, output));
    EXPECT_TRUE(output.size() == 1 && nly::geometry::equals(point, output.front()));

    output.clear();
    rect = make_polygon_rect(0, 0, 100, 100);
    EXPECT_TRUE(nly::geometry::intersection(point, rect, output));
    EXPECT_TRUE(output.size() == 1 && nly::geometry::equals(point, output.front()));

    point = { 50, 50 };
    output.clear();
    rect.inners().resize(1);
    auto& inner = rect.inners().front();
    inner.emplace_back(5, 5);
    inner.emplace_back(5, 95);
    inner.emplace_back(95, 95);
    inner.emplace_back(95, 5);
    nly::geometry::correct(rect);
    auto polygonArea = nly::geometry::area(rect);
    EXPECT_TRUE(nly::geometry::area(rect) == 100 * 100 - 90 * 90);
    EXPECT_TRUE(nly::geometry::intersection(point, rect, output) && output.empty());
  }

  // segment_ & segment_
  {
    auto                                segment = nly::geometry::make_segment(0, 0, 100, 0);
    std::vector<nly::geometry::point2d> output;

    EXPECT_TRUE(
      nly::geometry::intersection(segment, nly::geometry::make_segment(50, 30, 50, 90), output) &&
      output.empty());

    output.clear();
    EXPECT_TRUE(
      nly::geometry::intersection(segment, nly::geometry::make_segment(50, 0, 50, 30), output));
    EXPECT_TRUE(
      output.size() == 1 && nly::geometry::equals(nly::geometry::point2i(50, 0), output.front()));

    output.clear();
    EXPECT_TRUE(
      nly::geometry::intersection(segment, nly::geometry::make_segment(50, -30, 50, 30), output));
    EXPECT_TRUE(
      output.size() == 1 && nly::geometry::equals(nly::geometry::point2i(50, 0), output.front()));

    output.clear();
    EXPECT_TRUE(nly::geometry::intersection(segment, segment, output));
    EXPECT_TRUE(
      output.size() == 2 && nly::geometry::equals(nly::geometry::point2i(0, 0), output.front()));
    EXPECT_TRUE(
      output.size() == 2 && nly::geometry::equals(nly::geometry::point2i(100, 0), output.back()));
  }

  // linestring_ & linestring_
  {
    nly::geometry::linestring2i base;
    base.emplace_back(0, 0);
    base.emplace_back(0, 50);
    base.emplace_back(50, 50);

    std::vector<nly::geometry::point2d> output;

    nly::geometry::linestring2i linestring;
    EXPECT_TRUE(nly::geometry::intersection(base, linestring, output) && output.empty());

    output.clear();
    linestring.emplace_back(10, 10);
    EXPECT_TRUE(nly::geometry::intersection(base, linestring, output) && output.empty());

    output.clear();
    linestring.emplace_back(10, 20);
    EXPECT_TRUE(nly::geometry::intersection(base, linestring, output) && output.empty());

    output.clear();
    linestring.emplace_back(10, 50);
    EXPECT_TRUE(nly::geometry::intersection(base, linestring, output));
    EXPECT_TRUE(
      output.size() == 1 && nly::geometry::equals(nly::geometry::point2i(10, 50), output.front()));

    output.clear();
    linestring.emplace_back(50, 50);
    EXPECT_TRUE(nly::geometry::intersection(base, linestring, output));
    EXPECT_TRUE(output.size() == 3);
    EXPECT_TRUE(nly::geometry::equals(nly::geometry::point2i(10, 50), output.at(0)));
    EXPECT_TRUE(
      nly::geometry::equals(nly::geometry::point2i(10, 50), output.at(1))); // 此处不符合预期
    EXPECT_TRUE(nly::geometry::equals(nly::geometry::point2i(50, 50), output.at(2)));
  }

  // linestring_ & ring_
  {
    nly::geometry::linestring2i base;
    base.emplace_back(0, 0);
    base.emplace_back(0, 50);
    base.emplace_back(50, 50);

    std::vector<nly::geometry::point2d> output;
    EXPECT_TRUE(
      nly::geometry::intersection(base, nly::geometry::make_ring_rect(80, 80, 100, 100), output));

    output.clear();
    EXPECT_TRUE(
      nly::geometry::intersection(base, nly::geometry::make_ring_rect(50, 50, 100, 100), output));
    EXPECT_TRUE(
      output.size() == 1 && nly::geometry::equals(nly::geometry::point2i(50, 50), output.front()));

    output.clear();
    EXPECT_TRUE(
      nly::geometry::intersection(base, nly::geometry::make_ring_rect(40, 40, 100, 100), output));
    EXPECT_TRUE(
      output.size() == 1 && nly::geometry::equals(nly::geometry::point2i(40, 50), output.front()));

    output.clear();
    EXPECT_TRUE(
      nly::geometry::intersection(base, nly::geometry::make_ring_rect(40, 40, 100, 100), output));
    EXPECT_TRUE(
      output.size() == 1 && nly::geometry::equals(nly::geometry::point2i(40, 50), output.front()));

    output.clear();
    EXPECT_TRUE(
      nly::geometry::intersection(base, nly::geometry::make_ring_rect(-40, 40, 50, 50), output));
    EXPECT_TRUE(output.size() == 3);
    EXPECT_TRUE(nly::geometry::equals(nly::geometry::point2i(0, 50), output.at(0)));
    EXPECT_TRUE(nly::geometry::equals(nly::geometry::point2i(0, 40), output.at(1)));
    EXPECT_TRUE(nly::geometry::equals(nly::geometry::point2i(50, 50), output.at(2)));

    output.clear();
    EXPECT_TRUE(
      nly::geometry::intersection(
        base,
        nly::geometry::make_ring_rect(-100, -100, 100, 100),
        output));
    EXPECT_TRUE(output.empty());
  }

  // linestring_ & polygon_
  {
    nly::geometry::linestring2i base;
    base.emplace_back(0, 0);
    base.emplace_back(0, 50);
    base.emplace_back(50, 50);

    std::vector<nly::geometry::point2d> output;
    EXPECT_TRUE(nly::geometry::intersection(base, make_polygon_rect(80, 80, 100, 100), output));

    output.clear();
    EXPECT_TRUE(nly::geometry::intersection(base, make_polygon_rect(50, 50, 100, 100), output));
    EXPECT_TRUE(
      output.size() == 1 && nly::geometry::equals(nly::geometry::point2i(50, 50), output.front()));

    output.clear();
    EXPECT_TRUE(nly::geometry::intersection(base, make_polygon_rect(40, 40, 100, 100), output));
    EXPECT_TRUE(
      output.size() == 1 && nly::geometry::equals(nly::geometry::point2i(40, 50), output.front()));

    output.clear();
    EXPECT_TRUE(nly::geometry::intersection(base, make_polygon_rect(40, 40, 100, 100), output));
    EXPECT_TRUE(
      output.size() == 1 && nly::geometry::equals(nly::geometry::point2i(40, 50), output.front()));

    output.clear();
    EXPECT_TRUE(nly::geometry::intersection(base, make_polygon_rect(-40, 40, 50, 50), output));
    EXPECT_TRUE(output.size() == 3);
    EXPECT_TRUE(nly::geometry::equals(nly::geometry::point2i(0, 50), output.at(0)));
    EXPECT_TRUE(nly::geometry::equals(nly::geometry::point2i(0, 40), output.at(1)));
    EXPECT_TRUE(nly::geometry::equals(nly::geometry::point2i(50, 50), output.at(2)));

    output.clear();
    EXPECT_TRUE(nly::geometry::intersection(base, make_polygon_rect(-100, -100, 100, 100), output));
    EXPECT_TRUE(output.empty());

    output.clear();
    auto rect = make_polygon_rect(4, 4, 100, 100);
    rect.inners().resize(1);
    auto& inner = rect.inners().front();
    inner.emplace_back(5, 5);
    inner.emplace_back(5, 95);
    inner.emplace_back(95, 95);
    inner.emplace_back(95, 5);
    nly::geometry::correct(rect);
    EXPECT_TRUE(nly::geometry::intersection(base, rect, output));
    EXPECT_TRUE(output.size() == 2);
    EXPECT_TRUE(nly::geometry::equals(nly::geometry::point2i(4, 50), output.at(0)));
    EXPECT_TRUE(nly::geometry::equals(nly::geometry::point2i(5, 50), output.at(1)));
  }

  // rect_ & rect_
  {
    auto           rect = nly::geometry::make_rect(50, 50, 100, 100);
    decltype(rect) output;

    EXPECT_TRUE(!nly::geometry::intersection(rect, nly::geometry::make_rect(0, 0, 40, 40), output));

    EXPECT_TRUE(nly::geometry::intersection(rect, nly::geometry::make_rect(0, 0, 50, 50), output));
    EXPECT_TRUE(nly::geometry::equals(nly::geometry::make_rect(50, 50, 50, 50), output));

    EXPECT_TRUE(nly::geometry::intersection(rect, nly::geometry::make_rect(0, 0, 50, 80), output));
    EXPECT_TRUE(nly::geometry::equals(nly::geometry::make_rect(50, 50, 50, 80), output));

    EXPECT_TRUE(nly::geometry::intersection(rect, nly::geometry::make_rect(0, 0, 80, 80), output));
    EXPECT_TRUE(nly::geometry::equals(nly::geometry::make_rect(50, 50, 80, 80), output));

    EXPECT_TRUE(
      nly::geometry::intersection(rect, nly::geometry::make_rect(0, 0, 100, 100), output));
    EXPECT_TRUE(nly::geometry::equals(nly::geometry::make_rect(50, 50, 100, 100), output));

    EXPECT_TRUE(
      nly::geometry::intersection(rect, nly::geometry::make_rect(0, 0, 120, 120), output));
    EXPECT_TRUE(nly::geometry::equals(nly::geometry::make_rect(50, 50, 100, 100), output));
  }

  // rect_ & ring_
  {
    auto                  rect = nly::geometry::make_rect(50, 50, 100, 100);
    nly::geometry::ring2i output;

    EXPECT_TRUE(
      nly::geometry::intersection(rect, nly::geometry::make_ring_rect(0, 0, 40, 40), output));
    EXPECT_TRUE(output.empty());

    output.clear();
    EXPECT_TRUE(
      nly::geometry::intersection(rect, nly::geometry::make_ring_rect(0, 0, 50, 50), output));
    EXPECT_TRUE(nly::geometry::equals(nly::geometry::make_ring_rect(50, 50, 50, 50), output));

    output.clear();
    EXPECT_TRUE(
      nly::geometry::intersection(rect, nly::geometry::make_ring_rect(0, 0, 50, 80), output));
    EXPECT_TRUE(output.size() == 2);
    EXPECT_TRUE(nly::geometry::equals(output.at(0), nly::geometry::point2i(50, 80)));
    EXPECT_TRUE(nly::geometry::equals(output.at(1), nly::geometry::point2i(50, 50)));

    output.clear();
    EXPECT_TRUE(
      nly::geometry::intersection(rect, nly::geometry::make_ring_rect(0, 0, 80, 80), output));
    EXPECT_TRUE(output.size() == 2);
    EXPECT_TRUE(nly::geometry::equals(output.at(0), nly::geometry::point2i(50, 80)));
    EXPECT_TRUE(nly::geometry::equals(output.at(1), nly::geometry::point2i(80, 50)));

    output.clear();
    EXPECT_TRUE(
      nly::geometry::intersection(rect, nly::geometry::make_ring_rect(0, 0, 100, 100), output));
    EXPECT_TRUE(output.size() == 3);
    EXPECT_TRUE(nly::geometry::equals(output.at(0), nly::geometry::point2i(50, 100)));
    EXPECT_TRUE(nly::geometry::equals(output.at(1), nly::geometry::point2i(100, 100)));
    EXPECT_TRUE(nly::geometry::equals(output.at(2), nly::geometry::point2i(100, 50)));

    output.clear();
    EXPECT_TRUE(
      nly::geometry::intersection(rect, nly::geometry::make_ring_rect(0, 0, 120, 120), output));
    EXPECT_TRUE(output.empty());

    output.clear();
    EXPECT_TRUE(
      nly::geometry::intersection(
        nly::geometry::make_rect(-100, -100, 1000, 1000),
        nly::geometry::make_ring_rect(0, 0, 120, 120),
        output));
    EXPECT_TRUE(output.empty());
  }

  // rect_ & polygon
  {
    auto                  rect = nly::geometry::make_rect(50, 50, 100, 100);
    nly::geometry::ring2i output;

    EXPECT_TRUE(nly::geometry::intersection(rect, make_polygon_rect(0, 0, 40, 40), output));
    EXPECT_TRUE(output.empty());

    output.clear();
    EXPECT_TRUE(nly::geometry::intersection(rect, make_polygon_rect(0, 0, 50, 50), output));
    EXPECT_TRUE(output.size() == 1);
    EXPECT_TRUE(nly::geometry::equals(output.at(0), nly::geometry::point2i(50, 50)));

    output.clear();
    EXPECT_TRUE(nly::geometry::intersection(rect, make_polygon_rect(0, 0, 50, 80), output));
    EXPECT_TRUE(output.size() == 2);
    EXPECT_TRUE(nly::geometry::equals(output.at(0), nly::geometry::point2i(50, 80)));
    EXPECT_TRUE(nly::geometry::equals(output.at(1), nly::geometry::point2i(50, 50)));

    output.clear();
    EXPECT_TRUE(nly::geometry::intersection(rect, make_polygon_rect(0, 0, 80, 80), output));
    EXPECT_TRUE(output.size() == 2);
    EXPECT_TRUE(nly::geometry::equals(output.at(0), nly::geometry::point2i(50, 80)));
    EXPECT_TRUE(nly::geometry::equals(output.at(1), nly::geometry::point2i(80, 50)));

    output.clear();
    EXPECT_TRUE(nly::geometry::intersection(rect, make_polygon_rect(0, 0, 100, 100), output));
    EXPECT_TRUE(output.size() == 3);
    EXPECT_TRUE(nly::geometry::equals(output.at(0), nly::geometry::point2i(50, 100)));
    EXPECT_TRUE(nly::geometry::equals(output.at(1), nly::geometry::point2i(100, 100)));
    EXPECT_TRUE(nly::geometry::equals(output.at(2), nly::geometry::point2i(100, 50)));

    output.clear();
    EXPECT_TRUE(nly::geometry::intersection(rect, make_polygon_rect(0, 0, 120, 120), output));
    EXPECT_TRUE(output.empty());

    auto polygon = make_polygon_rect(80, 80, 110, 110);
    polygon.inners().resize(1);
    auto& inner = polygon.inners().front();
    inner.emplace_back(85, 85);
    inner.emplace_back(85, 105);
    inner.emplace_back(105, 105);
    inner.emplace_back(105, 85);
    nly::geometry::correct(polygon);
    EXPECT_TRUE(nly::geometry::intersection(rect, polygon, output));
    nly::geometry::correct(output);
    EXPECT_TRUE(output.size() == 5);
    EXPECT_TRUE(nly::geometry::equals(output.at(0), nly::geometry::point2i(80, 100)));
    EXPECT_TRUE(nly::geometry::equals(output.at(1), nly::geometry::point2i(85, 100)));
    EXPECT_TRUE(nly::geometry::equals(output.at(2), nly::geometry::point2i(100, 85)));
    EXPECT_TRUE(nly::geometry::equals(output.at(3), nly::geometry::point2i(100, 80)));
    EXPECT_TRUE(nly::geometry::equals(output.at(4), nly::geometry::point2i(80, 100)));
  }

  // ring_ & ring_
  {
    auto           rect = nly::geometry::make_ring_rect(50, 50, 100, 100);
    decltype(rect) output;

    EXPECT_TRUE(
      nly::geometry::intersection(rect, nly::geometry::make_ring_rect(0, 0, 40, 40), output));
    EXPECT_TRUE(output.empty());

    output.clear();
    EXPECT_TRUE(
      nly::geometry::intersection(rect, nly::geometry::make_ring_rect(0, 0, 50, 50), output));
    EXPECT_TRUE(output.size() == 1);
    EXPECT_TRUE(nly::geometry::equals(output.at(0), nly::geometry::point2i(50, 50)));

    output.clear();
    EXPECT_TRUE(
      nly::geometry::intersection(rect, nly::geometry::make_ring_rect(0, 0, 50, 80), output));
    EXPECT_TRUE(output.size() == 2);
    EXPECT_TRUE(nly::geometry::equals(output.at(0), nly::geometry::point2i(50, 80)));
    EXPECT_TRUE(nly::geometry::equals(output.at(1), nly::geometry::point2i(50, 50)));

    output.clear();
    EXPECT_TRUE(
      nly::geometry::intersection(rect, nly::geometry::make_ring_rect(0, 0, 80, 80), output));
    EXPECT_TRUE(output.size() == 2);
    EXPECT_TRUE(nly::geometry::equals(output.at(0), nly::geometry::point2i(50, 80)));
    EXPECT_TRUE(nly::geometry::equals(output.at(1), nly::geometry::point2i(80, 50)));

    output.clear();
    EXPECT_TRUE(
      nly::geometry::intersection(rect, nly::geometry::make_ring_rect(0, 0, 100, 100), output));
    EXPECT_TRUE(output.size() == 3);
    EXPECT_TRUE(nly::geometry::equals(output.at(0), nly::geometry::point2i(50, 100)));
    EXPECT_TRUE(nly::geometry::equals(output.at(1), nly::geometry::point2i(100, 100)));
    EXPECT_TRUE(nly::geometry::equals(output.at(2), nly::geometry::point2i(100, 50)));

    output.clear();
    EXPECT_TRUE(
      nly::geometry::intersection(rect, nly::geometry::make_ring_rect(0, 0, 120, 120), output));
    EXPECT_TRUE(output.empty());
  }

  // ring_ & polygon
  {
    auto                  rect = nly::geometry::make_ring_rect(50, 50, 100, 100);
    nly::geometry::ring2i output;

    EXPECT_TRUE(nly::geometry::intersection(rect, make_polygon_rect(0, 0, 40, 40), output));
    EXPECT_TRUE(output.empty());

    output.clear();
    EXPECT_TRUE(nly::geometry::intersection(rect, make_polygon_rect(0, 0, 50, 50), output));
    EXPECT_TRUE(output.size() == 1);
    EXPECT_TRUE(nly::geometry::equals(output.at(0), nly::geometry::point2i(50, 50)));

    output.clear();
    EXPECT_TRUE(nly::geometry::intersection(rect, make_polygon_rect(0, 0, 50, 80), output));
    EXPECT_TRUE(output.size() == 2);
    EXPECT_TRUE(nly::geometry::equals(output.at(0), nly::geometry::point2i(50, 80)));
    EXPECT_TRUE(nly::geometry::equals(output.at(1), nly::geometry::point2i(50, 50)));

    output.clear();
    EXPECT_TRUE(nly::geometry::intersection(rect, make_polygon_rect(0, 0, 80, 80), output));
    EXPECT_TRUE(output.size() == 2);
    EXPECT_TRUE(nly::geometry::equals(output.at(0), nly::geometry::point2i(50, 80)));
    EXPECT_TRUE(nly::geometry::equals(output.at(1), nly::geometry::point2i(80, 50)));

    output.clear();
    EXPECT_TRUE(nly::geometry::intersection(rect, make_polygon_rect(0, 0, 100, 100), output));
    EXPECT_TRUE(output.size() == 3);
    EXPECT_TRUE(nly::geometry::equals(output.at(0), nly::geometry::point2i(50, 100)));
    EXPECT_TRUE(nly::geometry::equals(output.at(1), nly::geometry::point2i(100, 100)));
    EXPECT_TRUE(nly::geometry::equals(output.at(2), nly::geometry::point2i(100, 50)));

    output.clear();
    EXPECT_TRUE(nly::geometry::intersection(rect, make_polygon_rect(0, 0, 120, 120), output));
    EXPECT_TRUE(output.empty());

    auto polygon = make_polygon_rect(80, 80, 110, 110);
    polygon.inners().resize(1);
    auto& inner = polygon.inners().front();
    inner.emplace_back(85, 85);
    inner.emplace_back(85, 105);
    inner.emplace_back(105, 105);
    inner.emplace_back(105, 85);
    nly::geometry::correct(polygon);
    EXPECT_TRUE(nly::geometry::intersection(rect, polygon, output));
    nly::geometry::correct(output);
    EXPECT_TRUE(output.size() == 5);
    EXPECT_TRUE(nly::geometry::equals(output.at(0), nly::geometry::point2i(80, 100)));
    EXPECT_TRUE(nly::geometry::equals(output.at(1), nly::geometry::point2i(85, 100)));
    EXPECT_TRUE(nly::geometry::equals(output.at(2), nly::geometry::point2i(100, 85)));
    EXPECT_TRUE(nly::geometry::equals(output.at(3), nly::geometry::point2i(100, 80)));
    EXPECT_TRUE(nly::geometry::equals(output.at(4), nly::geometry::point2i(80, 100)));
  }

  // polygon_ & polygon_
  {
    auto                                  polygon = make_polygon_rect(0, 0, 100, 100);
    std::vector<nly::geometry::polygon2i> output;

    polygon.inners().resize(1);
    auto& inner = polygon.inners().front();
    inner.emplace_back(40, 40);
    inner.emplace_back(40, 80);
    inner.emplace_back(80, 80);
    inner.emplace_back(80, 40);
    nly::geometry::correct(polygon);
    EXPECT_TRUE(nly::geometry::intersection(polygon, make_polygon_rect(50, 50, 90, 70), output));
    auto& outer = output.front().outer();
    EXPECT_TRUE(output.size() == 1);
    EXPECT_TRUE(outer.size() == 5);
    EXPECT_TRUE(nly::geometry::equals(outer.at(0), nly::geometry::point2i(80, 70)));
    EXPECT_TRUE(nly::geometry::equals(outer.at(1), nly::geometry::point2i(90, 70)));
    EXPECT_TRUE(nly::geometry::equals(outer.at(2), nly::geometry::point2i(90, 50)));
    EXPECT_TRUE(nly::geometry::equals(outer.at(3), nly::geometry::point2i(80, 50)));
    EXPECT_TRUE(nly::geometry::equals(outer.at(4), nly::geometry::point2i(80, 70)));

    output.clear();
    EXPECT_TRUE(
      nly::geometry::intersection(polygon, make_polygon_rect(1000, 1000, 2000, 2000), output));
    EXPECT_TRUE(output.empty());
  }
}

TEST(Geometry, Disjoint)
{
  nly::geometry::point2i point{};
  EXPECT_TRUE(!nly::geometry::disjoint(point, point));
  EXPECT_TRUE(!nly::geometry::disjoint(point, nly::geometry::make_rect(-100, -100, 100, 100)));
  EXPECT_TRUE(nly::geometry::disjoint(point, nly::geometry::make_rect(10, 10, 100, 100)));
  EXPECT_TRUE(!nly::geometry::disjoint(point, nly::geometry::make_rect(0, 0, 100, 100)));
  EXPECT_TRUE(!nly::geometry::disjoint(
    nly::geometry::point2i{ 100, 100 },
    nly::geometry::make_rect(0, 0, 100, 100)));
  EXPECT_TRUE(!nly::geometry::disjoint(
    nly::geometry::point2i{ 0, 50 },
    nly::geometry::make_rect(0, 0, 100, 100)));
  EXPECT_TRUE(!nly::geometry::disjoint(
    nly::geometry::make_rect(0, 0, 150, 150),
    nly::geometry::make_rect(100, 100, 200, 200)));
  EXPECT_TRUE(!nly::geometry::disjoint(
    nly::geometry::make_rect(0, 0, 100, 100),
    nly::geometry::make_rect(100, 100, 200, 200)));
  EXPECT_TRUE(
    nly::geometry::disjoint(
      nly::geometry::make_rect(0, 0, 99, 99),
      nly::geometry::make_rect(100, 100, 200, 200)));

  nly::geometry::polygon2i star;
  star.outer().emplace_back(0, 100);
  star.outer().emplace_back(-58, -81);
  star.outer().emplace_back(95, 31);
  star.outer().emplace_back(-95, 31);
  star.outer().emplace_back(58, -81);
  star.outer().emplace_back(0, 100);
  nly::geometry::correct(star);
  EXPECT_TRUE(!nly::geometry::disjoint(point, star));
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
