#include "gtest/gtest.h"
#include "nly/boost_helper/serialization.hpp"
#include "boost/serialization/vector.hpp"
#include <vector>

class hello
{
  friend class boost::serialization::access;

public:
  template<class Archive>
  void serialize(Archive& ar, const unsigned int)
  {
    ar & m_int;
    ar & m_str;
    ar & m_data;
  }

public:
  int               m_int{ 0 };
  std::string       m_str{ "hello" };
  std::vector<char> m_data;
};

TEST(Serialization, Binary)
{
  {
    nly::streambuf buff;

    // 放点垃圾数据进去
    buff.prepare_output(1024);
    buff.commit_output(1024);

    const size_t count = 1024LL * 1024 * 1024 * 2.1;

    {
      hello h;
      h.m_int = 1024;
      h.m_str = "szn";
      h.m_data.resize(count);

      nly::serialization::to_binary(h, buff);
    }

    {
      hello h;
      EXPECT_TRUE(nly::serialization::from_binary(h, buff));

      EXPECT_TRUE(h.m_int == 1024);
      EXPECT_TRUE(h.m_str == "szn");
      EXPECT_TRUE(h.m_data.size() == count);
    }
  }
}

TEST(Serialization, Text)
{
  hello h;
  h.m_int = 1024;
  h.m_str = "szn";

  auto content = nly::serialization::to_text(h);
  EXPECT_TRUE(!content.empty());

  auto h1 = nly::serialization::from_text<hello>(content);
  EXPECT_TRUE(h1.has_value());
  EXPECT_EQ(h1->m_int, 1024);
  EXPECT_EQ(h1->m_str, "szn");

  auto h2 = nly::serialization::from_text<hello>("");
  EXPECT_TRUE(!h2.has_value());
}