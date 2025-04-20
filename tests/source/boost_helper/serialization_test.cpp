#include "gtest/gtest.h"
#include "nly/boost_helper/serialization.hpp"

class hello
{
  friend class boost::serialization::access;

public:
  template<class Archive>
  void serialize(Archive& ar, const unsigned int version)
  {
    ar & m_int;
    ar & m_str;
  }

public:
  int         m_int{ 0 };
  std::string m_str{ "hello" };
};

TEST(Serialization, binary)
{
  hello h;
  h.m_int = 1024;
  h.m_str = "szn";

  for (int i = 0; i < 2; ++i)
  {
    auto content = nly::serialization::to_data(h, i == 0);
    EXPECT_TRUE(!content.empty());

    auto h1 = nly::serialization::from_data<hello>(content, i == 0);
    EXPECT_TRUE(h1.has_value());
    EXPECT_EQ(h1->m_int, 1024);
    EXPECT_EQ(h1->m_str, "szn");

    auto h2 = nly::serialization::from_data<hello>("", i == 0);
    EXPECT_TRUE(!h2.has_value());
  }
}