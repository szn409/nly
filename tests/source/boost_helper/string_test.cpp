#include "gtest/gtest.h"
#include "nly/boost_helper/string.hpp"
#include <string>
#include <vector>

using std::string;
using std::u16string;
using std::u32string;
using std::vector;
using std::wstring;

TEST(String, Split)
{
  auto fun = [](const char value) { return value == ';'; };

  vector<string> vec;
  nly::string_algo::split("12;34;56", vec, ';');
  EXPECT_TRUE(vec == (vector<string>{ "12", "34", "56" }));
  nly::string_algo::split_if("12;34;56", vec, fun);
  EXPECT_TRUE(vec == (vector<string>{ "12", "34", "56" }));

  nly::string_algo::split("12;34;56;", vec, ';');
  EXPECT_TRUE(vec == (vector<string>{ "12", "34", "56", "" }));
  nly::string_algo::split_if("12;34;56;", vec, fun);
  EXPECT_TRUE(vec == (vector<string>{ "12", "34", "56", "" }));

  nly::string_algo::split("12;34;;56", vec, ';');
  EXPECT_TRUE(vec == (vector<string>{ "12", "34", "", "56" }));
  nly::string_algo::split_if("12;34;;56", vec, fun);
  EXPECT_TRUE(vec == (vector<string>{ "12", "34", "", "56" }));

  nly::string_algo::split("12;34;;;;56", vec, ';', true);
  EXPECT_TRUE(vec == (vector<string>{ "12", "34", "56" }));
  nly::string_algo::split_if("12;34;;;;56", vec, fun, true);
  EXPECT_TRUE(vec == (vector<string>{ "12", "34", "56" }));

  vector<wstring> out_w;
  nly::string_algo::split(L"12;34;56", out_w, L';');
  EXPECT_TRUE(out_w == (vector<wstring>{ L"12", L"34", L"56" }));
  nly::string_algo::split_if(L"12;34;56", out_w, [](auto v) { return v == L';'; });
  EXPECT_TRUE(out_w == (vector<wstring>{ L"12", L"34", L"56" }));

  vector<u16string> out_16;
  nly::string_algo::split(u"12;34;56", out_16, u';');
  EXPECT_TRUE(out_16 == (vector<u16string>{ u"12", u"34", u"56" }));
  nly::string_algo::split_if(u"12;34;56", out_16, [](auto v) { return v == u';'; });
  EXPECT_TRUE(out_16 == (vector<u16string>{ u"12", u"34", u"56" }));

  vector<u32string> out_32;
  nly::string_algo::split(U"12;34;56", out_32, U';');
  EXPECT_TRUE(out_32 == (vector<u32string>{ U"12", U"34", U"56" }));
  nly::string_algo::split_if(U"12;34;56", out_32, [](auto v) { return v == U';'; });
  EXPECT_TRUE(out_32 == (vector<u32string>{ U"12", U"34", U"56" }));
}