#include "gtest/gtest.h"
#include "nly/boost_helper/asio_helper/streambuf.hpp"

TEST(Streambuf, All)
{
  {
    nly::streambuf buf(1024);
    EXPECT_TRUE(buf.max_size() == 1024);
    EXPECT_TRUE(128 == buf.capacity());

    auto data = buf.get_input();
    EXPECT_TRUE(0 == data.size());
  }

  {
    nly::streambuf buf(100);
    EXPECT_TRUE(buf.max_size() == 100);
    EXPECT_TRUE(100 == buf.capacity());
  }

  {
    const size_t count = 1024LL * 1024 * 1024 * 2.2;

    nly::streambuf buf;
    buf.prepare_output(count);
    EXPECT_TRUE(0 == buf.get_input().size());

    buf.commit_output(1024);
    EXPECT_TRUE(1024 == buf.get_input().size());

    buf.commit_output(static_cast<size_t>(-1));
    EXPECT_TRUE(count == buf.get_input().size());

    buf.consume_input(1024);
    EXPECT_TRUE(count - 1024 == buf.get_input().size());

    buf.consume_input(static_cast<size_t>(-1));
    EXPECT_TRUE(0 == buf.get_input().size());
  }

  {
    nly::streambuf my_data;
    auto&          buf = my_data.get();

    std::ostream os(&buf);
    std::istream is(&buf);

    os << 1;
    os << "hello";
    EXPECT_TRUE(my_data.get_input().size() == 6);
    EXPECT_TRUE(static_cast<const char*>(my_data.get_input().data()) == std::string("1hello"));

    std::string str;
    is >> str;
    EXPECT_TRUE(0 == my_data.get_input().size());
    EXPECT_TRUE(str == "1hello");

    EXPECT_TRUE(is.eof());

    // 得执行下 clear, 因为其已经触发 EOF
    is.clear();

    os << "hello";
    my_data.consume_input(2);
    EXPECT_TRUE(3 == my_data.get_input().size());
    is >> str;
    EXPECT_TRUE(str == "llo");
  }
}