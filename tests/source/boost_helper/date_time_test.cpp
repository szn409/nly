#include "gtest/gtest.h"
#include "nly/boost_helper/date_time.hpp"

TEST(DateTime, Date)
{
  EXPECT_TRUE(
    nly::boost_date_time::to_string(nly::boost_date_time::make_date(2020, 1, 20)) == "2020-01-20");
  EXPECT_TRUE(nly::boost_date_time::to_string(nly::boost_date_time::max_date()) == "9999-12-31");
  EXPECT_TRUE(nly::boost_date_time::to_string(nly::boost_date_time::min_date()) == "1400-01-01");
  EXPECT_TRUE(
    nly::boost_date_time::to_undelimited_string(nly::boost_date_time::make_date(2020, 1, 20)) ==
    "20200120");

  const auto value = nly::boost_date_time::make_date(2020, 1, 20);
  EXPECT_TRUE(value == nly::boost_date_time::date_from_string("2020,01-20"));
  EXPECT_TRUE(value == nly::boost_date_time::date_from_string("2020.01 20"));
  EXPECT_TRUE(value == nly::boost_date_time::date_from_string("2020/01/20"));
  EXPECT_TRUE(value == nly::boost_date_time::date_from_undelimited_string("20200120"));

  tm t = nly::boost_date_time::date_to_tm(value);
  EXPECT_TRUE(t.tm_year == value.year() - 1900);
  EXPECT_TRUE(t.tm_mon == value.month() - 1);
  EXPECT_TRUE(t.tm_mday == value.day());
  EXPECT_TRUE(nly::boost_date_time::date_from_tm(t) == value);
}

TEST(DateTime, TimeDuration)
{
  const auto value = nly::boost_date_time::make_time_duration(30, 70, 80, 1234567);
  EXPECT_TRUE(nly::boost_date_time::to_string(value) == "31:11:21.234567");
  EXPECT_TRUE(
    nly::boost_date_time::to_string(nly::boost_date_time::make_time_duration(30, 70, 80)) ==
    "31:11:20");
  EXPECT_TRUE(nly::boost_date_time::to_undelimited_string(value) == "311121.234567");
  EXPECT_TRUE(
    nly::boost_date_time::to_undelimited_string(
      nly::boost_date_time::make_time_duration(30, 70, 80)) == "311120");

  EXPECT_TRUE(value == nly::boost_date_time::time_duration_from_string("31-11:21,234567"));
  EXPECT_TRUE(value == nly::boost_date_time::time_duration_from_string("31.11.21,234567"));

  auto t = nly::boost_date_time::to_tm(value);
  EXPECT_TRUE(!t.tm_year && !t.tm_mon && !t.tm_mday);
  EXPECT_TRUE(t.tm_hour == 31);
  EXPECT_TRUE(t.tm_min == 11);
  EXPECT_TRUE(t.tm_sec == 21);
}

TEST(DateTime, PTime)
{
  const auto date = nly::boost_date_time::make_date(2020, 1, 30);
  const auto time = nly::boost_date_time::make_time_duration(1, 2, 3, 4);
  const auto date_time = nly::boost_date_time::make_ptime(2020, 1, 30, 1, 2, 3, 4);
  EXPECT_TRUE(
    nly::boost_date_time::to_string(nly::boost_date_time::make_ptime(date)) ==
    "2020-01-30 00:00:00");
  EXPECT_TRUE(
    nly::boost_date_time::to_string(nly::boost_date_time::make_ptime(date, time)) ==
    "2020-01-30 01:02:03.000004");
  EXPECT_TRUE(nly::boost_date_time::to_string(date_time) == "2020-01-30 01:02:03.000004");

  EXPECT_TRUE(nly::boost_date_time::ptime_from_string("2020-1-30T1:02:03.000004") == date_time);
  EXPECT_TRUE(nly::boost_date_time::ptime_from_string("2020-1-30 1:02:03.000004") == date_time);
  EXPECT_TRUE(nly::boost_date_time::ptime_from_string("2020-01-30 01:02:03.000004") == date_time);
  EXPECT_TRUE(nly::boost_date_time::ptime_from_string("2020-01-30T01:02:03.000004") == date_time);

  auto t = nly::boost_date_time::to_tm(date_time);
  EXPECT_TRUE(t.tm_year == date_time.date().year() - 1900);
  EXPECT_TRUE(t.tm_mon == date_time.date().month() - 1);
  EXPECT_TRUE(t.tm_mday == date_time.date().day());
  EXPECT_TRUE(t.tm_hour == date_time.time_of_day().hours());
  EXPECT_TRUE(t.tm_min == date_time.time_of_day().minutes());
  EXPECT_TRUE(t.tm_sec == date_time.time_of_day().seconds());

  EXPECT_TRUE(
    nly::boost_date_time::ptime_from_tm(t) ==
    nly::boost_date_time::make_ptime(2020, 1, 30, 1, 2, 3));
}
